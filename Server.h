#pragma once
#include "ServerConfig.h"
#include "ResHeader.h"

#define OP_READ   1
#define OP_WRITE  2
#define OP_ACCEPT 3
#define DataBuffSize 1024

typedef struct
{
	OVERLAPPED overlapped;
	WSABUF databuff;
	char buffer[DataBuffSize];
	DWORD length;
	int operationType;
}PER_IO_OPERATEION_DATA, *LPPER_IO_OPERATION_DATA, *LPPER_IO_DATA, PER_IO_DATA;

/**
* 结构体名称：PER_HANDLE_DATA
* 结构体存储：记录单个套接字的数据，包括了套接字的变量及套接字的对应的客户端的地址。
* 结构体作用：当服务器连接上客户端时，信息存储到该结构体中，知道客户端的地址以便于回访。
**/
typedef struct
{
	SOCKET socket;
	SOCKADDR_STORAGE ClientAddr;
	SYSTEMTIME lastRecv;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;
typedef struct ThreadPars {
	int count;
	SOCKET sockets[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT events[WSA_MAXIMUM_WAIT_EVENTS];
} ThreadPars;

typedef struct ThreadParsL {
	ThreadPars* threadPars;
	ThreadParsL* next;
} ThreadParsL;
class Server
{
public:
	Server(Config* config);
	~Server();
private:
	SOCKET sockSrv;
	vector < PER_HANDLE_DATA* > clientGroup;
};