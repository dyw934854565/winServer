#include "stdafx.h"
#include "TcpServer.h"
TcpServer::TcpServer()
{
	//this->CreatServer();
}
// 开始服务工作线程函数
unsigned int __stdcall ServerWorkThread(LPVOID IpParam)
{
	TcpServer* server = (TcpServer*)IpParam;
	HANDLE CompletionPort = server->getCompletionPort();
	DWORD BytesTransferred;
	LPOVERLAPPED IpOverlapped;
	LPPER_HANDLE_DATA PerHandleData = NULL;
	LPPER_IO_DATA PerIoData = NULL;
	BOOL bRet = false;
	LPPER_HANDLE_DATA newHandleData;
	while (true) {
		bRet = GetQueuedCompletionStatus(CompletionPort, &BytesTransferred, (PULONG_PTR)&PerHandleData, (LPOVERLAPPED*)&IpOverlapped, INFINITE);
		if (bRet == 0) {
			cerr << "GetQueuedCompletionStatus Error: " << GetLastError() << endl;
			return -1;
		}
		PerIoData = (LPPER_IO_DATA)CONTAINING_RECORD(IpOverlapped, PER_IO_DATA, overlapped);

		// 检查在套接字上是否有错误发生
		if (0 == BytesTransferred && (PerIoData->operationType == OP_READ || PerIoData->operationType == OP_WRITE)) {
			server->OnClose(PerHandleData);
			closesocket((SOCKET)PerIoData->ClientHandle);

			delete PerIoData;
			PerIoData = NULL;

			delete PerHandleData;
			PerHandleData = NULL;
			continue;
		}

		switch (PerIoData->operationType)
		{
		case OP_ACCEPT:    // 完成一个连接请求
			newHandleData = new PER_HANDLE_DATA();
			newHandleData->handle = PerIoData->ClientHandle;
			CreateIoCompletionPort(newHandleData->handle, CompletionPort, (DWORD)newHandleData, 0);
			if (server->OnAccept((SOCKET)newHandleData->handle, PerIoData)) {
				//投递接收请求
				server->recv((SOCKET)newHandleData->handle, PerIoData);
			}
			//投递连接
			server->accept();
			break;
		case OP_READ:    // 完成一个接收请求
			GetSystemTime(&(PerHandleData->lastRecv));
			if (server->OnRecv((SOCKET)PerHandleData->handle, PerIoData))
			{
				//投递接收请求
				server->recv((SOCKET)PerHandleData->handle, PerIoData);
			}

			//cout << "A Client says: " << PerIoData->databuff.buf << endl;

			//http->handle(PerIoData, PerHandleData->socket, CompletionPort);

			/*
			sprintf_s(PerIoData->buffer, DataBuffSize, "%s%s", header, content);
			PerIoData->databuff.len = strlen(PerIoData->buffer);
			PerIoData->databuff.buf = PerIoData->buffer;
			PerIoData->operationType = OP_WRITE;    // read
			WSASend(PerHandleData->socket, &(PerIoData->databuff), 1, &(PerIoData->length), Flags, &(PerIoData->overlapped), NULL);
			*/
			break;
		case OP_WRITE:   // 完成一个发送请求
			GetSystemTime(&(PerHandleData->lastSend));

			if (server->OnSend((SOCKET)PerHandleData->handle, PerIoData))
			{
				//投递接收请求
				server->recv((SOCKET)PerHandleData->handle, PerIoData);
			}
			break;
		default:
			server->OnComplite(PerIoData->operationType, PerHandleData, PerIoData);
			break;
		}
	}
	return 0;
}
BOOL TcpServer::OnAccept(SOCKET clientSock, LPPER_IO_DATA PerIoData)
{
	cout << "on accept" << endl;
	return true;
}
BOOL TcpServer::OnRecv(SOCKET clientSock, LPPER_IO_DATA PerIoData)
{
	cout << PerIoData->databuff.buf << endl;
	cout << "on recv" << endl;
	return true;
}
BOOL TcpServer::OnSend(SOCKET clientSock, LPPER_IO_DATA PerIoData)
{
	cout << "on send" << endl;
	return true;
}
BOOL TcpServer::OnClose(LPPER_HANDLE_DATA PerHandleData)
{
	cout << "on close" << endl;
	return true;
}
void TcpServer::accept()
{
	LPPER_IO_DATA PerIoData = new PER_IO_DATA();

	this->accept(PerIoData);
}
void TcpServer::accept(LPPER_IO_DATA PerIoData)
{
	ZeroMemory(PerIoData, sizeof(PER_IO_DATA)); // 清空内存
	PerIoData->ClientHandle = (HANDLE)WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	PerIoData->operationType = OP_ACCEPT;
	//将接收缓冲置为0,令AcceptEx直接返回,防止拒绝服务攻击 
	AcceptEx(this->sockSrv, (SOCKET)PerIoData->ClientHandle, PerIoData->buffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &(PerIoData->length), &(PerIoData->overlapped));
}
void TcpServer::recv(SOCKET clientSock)
{
	//接收消息
	LPPER_IO_DATA newPerIoData = new PER_IO_DATA();
	this->recv(clientSock, newPerIoData);
}
void TcpServer::recv(SOCKET clientSock, LPPER_IO_DATA PerIoData)
{
	ZeroMemory(PerIoData, sizeof(PER_IO_DATA)); // 清空内存 

	PerIoData->databuff.len = DataBuffSize;
	PerIoData->databuff.buf = PerIoData->buffer;
	PerIoData->operationType = OP_READ;
	DWORD flags = 0;
	WSARecv(clientSock, &(PerIoData->databuff), 1, &PerIoData->length, &flags, &(PerIoData->overlapped), NULL);
}
void TcpServer::send(SOCKET clientSock, LPPER_IO_DATA PerIoData)
{
	PerIoData->operationType = OP_WRITE;
	PerIoData->databuff.buf = PerIoData->buffer;
	PerIoData->databuff.len = strlen(PerIoData->buffer);
	WSASend(clientSock, &(PerIoData->databuff), 1, &(PerIoData->length), NULL, &(PerIoData->overlapped), NULL);
}
HANDLE TcpServer::getCompletionPort()
{
	return this->completionPort;
}
void TcpServer::OnComplite(int type, LPPER_HANDLE_DATA PerHandleData, LPPER_IO_DATA PerIoData)
{
	cout << type << endl;
}
void TcpServer::CreatServer()
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
	this->completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (NULL == this->completionPort) {    // 创建IO内核对象失败  
		cerr << "CreateIoCompletionPort failed. Error:" << GetLastError() << endl;
		return;
	}

	// 创建IOCP线程--线程里面创建线程池  

	// 基于处理器的核心数量创建线程  
	for (DWORD i = 0; i < (mySysInfo.dwNumberOfProcessors * 2); ++i) {
		// 创建服务器工作器线程，并将完成端口传递到该线程  
		HANDLE ThreadHandle = (HANDLE)_beginthreadex(NULL, 0, ServerWorkThread, (PVOID)this, 0, NULL);
		if (NULL == ThreadHandle) {
			cerr << "Create Thread Handle failed. Error:" << GetLastError() << endl;
			return;
		}
		CloseHandle(ThreadHandle);
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(this->port);

	err = bind(this->sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (err != 0)
	{
		cout << "socket bind fail" << err << endl;
		WSACleanup();
		return;
	}
	err = listen(this->sockSrv, SOMAXCONN);
	if (err != 0)
	{
		cout << "socket listen fail" << err << endl;
		WSACleanup();
		return;
	}
	cout << "sever listen on port:" << this->port << endl;

	LPPER_HANDLE_DATA handleData = new PER_HANDLE_DATA();
	handleData->handle = (HANDLE)this->sockSrv;
	CreateIoCompletionPort((HANDLE)(this->sockSrv), this->completionPort, (DWORD)handleData, 0);

	for (int i = 0; i < 100; i++)
	{
		this->accept();
	}
}
TcpServer::~TcpServer()
{
	//todo 释放客户端vector
	closesocket(this->sockSrv);
	WSACleanup();
}