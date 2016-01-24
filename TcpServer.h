#pragma once
#include "iocpData.h"
#include "mswsock.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Mswsock.lib")

class TcpServer
{
public:
	TcpServer();
	virtual BOOL OnAccept(SOCKET clientSock, LPPER_IO_DATA PerIoData);
	virtual BOOL OnRecv(SOCKET clientSock, LPPER_IO_DATA PerIoData);
	virtual BOOL OnSend(SOCKET clientSock, LPPER_IO_DATA PerIoData);
	virtual BOOL OnClose(LPPER_HANDLE_DATA PerHandleData);

	virtual void OnComplite(int type, LPPER_HANDLE_DATA PerHandleData, LPPER_IO_DATA PerIoData, DWORD BytesTransferred);

	//Ͷ��iocp����
	void recv(SOCKET clientSock, LPPER_IO_DATA PerIoData);

	//Ͷ��iocp����
	void send(SOCKET clientSock, LPPER_IO_DATA PerIoData, int length = 0);

	//Ͷ��iocp����
	void accept();
	void clientHandler();
	//��ȡ��ɶ˿�
	HANDLE getCompletionPort();
	~TcpServer();
private:
	void recv(SOCKET clientSock);
	void accept(LPPER_IO_DATA PerIoData);
	vector < PER_HANDLE_DATA* > clientGroup;
protected:
	void CreatServer();
	SOCKET sockSrv;
	HANDLE completionPort;
	int port = 80;
};