#include "stdafx.h"
#include "Server.h"
/*
unsigned int __stdcall ClientHandlerThreadFun(PVOID pM)
{
	int ret;
	int Index;
	ThreadPars* threadPars = (ThreadPars*)pM;
	while (1)
	{
		if (threadPars->count > 0)
		{
			ret = WSAWaitForMultipleEvents(threadPars->count, threadPars->events, FALSE, 100, FALSE);
			if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)
			{
				
			}
			else
			{
				Index = ret - WSA_WAIT_EVENT_0;
				for (int i = Index; i <= threadPars->count; i++)
				{
					WSAResetEvent(threadPars->events[i]);
					WSANETWORKEVENTS netEvent;
					WSAEnumNetworkEvents(threadPars->sockets[i], threadPars->events[i], &netEvent);
					if (netEvent.lNetworkEvents & FD_READ)                // 处理FD_READ通知消息  
					{
						cout << "new request" << endl;
						char recvBuf[500];
						recv(threadPars->sockets[i], recvBuf, 500, 0);
						cout << recvBuf << endl;
						char sendBuf[5000];
						char content[1000] = "这是内容";
						char header[500] = "HTTP/1.1 200 OK\r\nDate: Thu, 24 Dec 2015 13:36:34 GMT\r\nServer: winServer by dyw\r\nConnection: Keep-Alive\r\nContent-Length: 8\r\nContent-Type: text/html;charset=UTF-8\r\n\r\n";
						sprintf_s(sendBuf, "%s%s", header, content);
						send(threadPars->sockets[i], sendBuf, strlen(sendBuf) + 1, 0);
					}
					else if (netEvent.lNetworkEvents & FD_CLOSE)
					{
						cout << "client close" << endl;
					}
				}
			}
		}
	}
	return 0;
}
*/
// 开始服务工作线程函数  
unsigned int __stdcall ServerWorkThread(LPVOID IpParam)
{
	HANDLE CompletionPort = (HANDLE)IpParam;
	DWORD BytesTransferred;
	LPOVERLAPPED IpOverlapped;
	LPPER_HANDLE_DATA PerHandleData = NULL;
	LPPER_IO_DATA PerIoData = NULL;
	DWORD RecvBytes;
	DWORD Flags = 0;
	BOOL bRet = false;

	while (true) {
		bRet = GetQueuedCompletionStatus(CompletionPort, &BytesTransferred, (PULONG_PTR)&PerHandleData, (LPOVERLAPPED*)&IpOverlapped, INFINITE);
		if (bRet == 0) {
			cerr << "GetQueuedCompletionStatus Error: " << GetLastError() << endl;
			return -1;
		}
		PerIoData = (LPPER_IO_DATA)CONTAINING_RECORD(IpOverlapped, PER_IO_DATA, overlapped);

		// 检查在套接字上是否有错误发生  
		if (0 == BytesTransferred && (PerIoData->operationType == OP_READ || PerIoData->operationType == OP_WRITE)) {
			//cout << "close socket" << endl;
			closesocket(PerHandleData->socket);
			delete PerHandleData;
			PerHandleData = NULL;
			delete PerIoData;
			PerIoData = NULL;
			continue;
		}
		char content[1000] = "这是内容\r\n";
		//cout << strlen(content) << endl;
		char time[100];
		ResHeader* resheader = new ResHeader("ffe");
		resheader->getUTCstr(time);
		char header[500];
		sprintf_s(header, 500, "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nDate: %s\r\nServer: winServer by dyw\r\nContent-Length: 10\r\nContent-Type: text/plain\r\n\r\n", time);
		delete resheader;
		switch (PerIoData->operationType)
		{
			case OP_READ:       // 完成一个接收请求
				// 开始数据处理，接收来自客户端的数据  
				
				//cout << "A Client says: " << PerIoData->databuff.buf << endl;
				GetSystemTime(&(PerHandleData->lastRecv));
				ZeroMemory(PerIoData, sizeof(PER_IO_DATA)); // 清空内存 

				sprintf_s(PerIoData->buffer, "%s%s", header, content);
				PerIoData->databuff.len = strlen(PerIoData->buffer);
				PerIoData->databuff.buf = PerIoData->buffer;
				PerIoData->operationType = OP_WRITE;    // read
				WSASend(PerHandleData->socket, &(PerIoData->databuff), 1, &(PerIoData->length), Flags, &(PerIoData->overlapped), NULL);
				//ReleaseMutex(hMutex);
				break;
			case OP_WRITE:
				// 为下一个重叠调用建立单I/O操作数据  
				//cout << "write success" << endl;
				//cout << BytesTransferred << PerIoData->length << endl;
				ZeroMemory(PerIoData, sizeof(PER_IO_DATA)); // 清空内存  
				PerIoData->databuff.len = DataBuffSize;
				PerIoData->databuff.buf = PerIoData->buffer;
				PerIoData->operationType = OP_READ;    // read  
				WSARecv(PerHandleData->socket, &(PerIoData->databuff), 1, &RecvBytes, &Flags, &(PerIoData->overlapped), NULL);
				break;
		}
	}

	return 0;
}
Server::Server(Config* config)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	int sockaddrlen = sizeof(SOCKADDR);
	// 确定处理器的核心数量  
	SYSTEM_INFO mySysInfo;
	GetSystemInfo(&mySysInfo);

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		cout << "socket init fail" << endl;
		return;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		cout << "no socket version" << endl;
		WSACleanup();
		return;
	}
	this->sockSrv = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

	// 创建IOCP的内核对象  
	/**
	* 需要用到的函数的原型：
	* HANDLE WINAPI CreateIoCompletionPort(
	*    __in   HANDLE FileHandle,     // 已经打开的文件句柄或者空句柄，一般是客户端的句柄
	*    __in   HANDLE ExistingCompletionPort, // 已经存在的IOCP句柄
	*    __in   ULONG_PTR CompletionKey,   // 完成键，包含了指定I/O完成包的指定文件
	*    __in   DWORD NumberOfConcurrentThreads // 真正并发同时执行最大线程数，一般推介是CPU核心数*2
	* );
	**/
	HANDLE completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (NULL == completionPort) {    // 创建IO内核对象失败  
		cerr << "CreateIoCompletionPort failed. Error:" << GetLastError() << endl;
		return;
	}

	// 创建IOCP线程--线程里面创建线程池  

	// 基于处理器的核心数量创建线程  
	for (DWORD i = 0; i < (mySysInfo.dwNumberOfProcessors * 2); ++i) {
		// 创建服务器工作器线程，并将完成端口传递到该线程  
		HANDLE ThreadHandle = (HANDLE)_beginthreadex(NULL, 0, ServerWorkThread, (PVOID)completionPort, 0, NULL);
		if (NULL == ThreadHandle) {
			cerr << "Create Thread Handle failed. Error:" << GetLastError() << endl;
			return;
		}
		CloseHandle(ThreadHandle);
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(config->getPort());

	err = bind(this->sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (err != 0)
	{
		cout << "socket bind fail"<< err << endl;
		WSACleanup();
		return;
	}
	WSAEVENT sockSrvEvent = WSACreateEvent();
	WSAEventSelect(this->sockSrv, sockSrvEvent, FD_ACCEPT | FD_CLOSE);// | FD_READ);
	err = listen(this->sockSrv, SOMAXCONN);
	if (err != 0)
	{
		cout << "socket listen fail" << err << endl;
		WSACleanup();
		return;
	}
	cout << "sever listen on port:" << config->getPort() << endl;
	while (1)
	{
		int ret = WSAWaitForMultipleEvents(1, &sockSrvEvent, TRUE, WSA_INFINITE, FALSE);
		
		if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)
		{
		}
		else
		{
			WSAResetEvent(sockSrvEvent);
			WSANETWORKEVENTS netEvent;
			WSAEnumNetworkEvents(this->sockSrv, sockSrvEvent, &netEvent);
			if (netEvent.lNetworkEvents & FD_ACCEPT)                // 处理FD_ACCEPT通知消息  
			{
				//cout << "new connetion" << endl;
				SOCKADDR_IN ClientAddr;
				
				SOCKET acceptSocket = accept(this->sockSrv, (SOCKADDR*)&ClientAddr, &sockaddrlen);
				PER_HANDLE_DATA* PerHandleData = new PER_HANDLE_DATA();

				PerHandleData->socket = acceptSocket;
				memcpy(&PerHandleData->ClientAddr, &ClientAddr, sockaddrlen);
				this->clientGroup.push_back(PerHandleData);

				CreateIoCompletionPort((HANDLE)(PerHandleData->socket), completionPort, (DWORD)PerHandleData, 0);

				LPPER_IO_OPERATION_DATA PerIoData = new PER_IO_OPERATEION_DATA();
				ZeroMemory(&(PerIoData->overlapped), sizeof(OVERLAPPED));
				PerIoData->databuff.len = DataBuffSize;
				PerIoData->databuff.buf = PerIoData->buffer;
				PerIoData->operationType = OP_READ;    // read  

				DWORD Flags = 0;
				WSARecv(PerHandleData->socket, &(PerIoData->databuff), 1, &(PerIoData->length), &Flags, &(PerIoData->overlapped), NULL);
			}
			else if (netEvent.lNetworkEvents & FD_CLOSE)
			{
				cout << "bye" << endl;
				WSACloseEvent(sockSrvEvent);
			}
		}
	}
	
}
Server::~Server()
{
	//todo 释放客户端vector
	closesocket(this->sockSrv);
	WSACleanup();
}