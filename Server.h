#pragma once
#include "ServerConfig.h"
#include "iocpData.h"
#include "mswsock.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Mswsock.lib")

class Server
{
public:
	Server();
	virtual BOOL OnAccept(SOCKET clientSock, LPPER_IO_DATA PerIoData);
	virtual BOOL OnRecv(SOCKET clientSock, LPPER_IO_DATA PerIoData);
	virtual BOOL OnSend(SOCKET clientSock, LPPER_IO_DATA PerIoData);
	virtual BOOL OnClose(LPPER_HANDLE_DATA PerHandleData);

	//Ͷ��iocp����
	void recv(SOCKET clientSock, LPPER_IO_DATA PerIoData);

	//Ͷ��iocp����
	void send(SOCKET clientSock, LPPER_IO_DATA PerIoData);

	//Ͷ��iocp����
	void accept();

	//��ȡ��ɶ˿�
	HANDLE getCompletionPort();
	void OnComplite(int type);
	~Server();
private:
	void recv(SOCKET clientSock);
	void accept(LPPER_IO_DATA PerIoData);
	vector < PER_HANDLE_DATA* > clientGroup;
protected:
	void CreatServer();
	HANDLE completionPort;
	SOCKET sockSrv;
	int port = 80;
};