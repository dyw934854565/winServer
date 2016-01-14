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
* �ṹ�����ƣ�PER_HANDLE_DATA
* �ṹ��洢����¼�����׽��ֵ����ݣ��������׽��ֵı������׽��ֵĶ�Ӧ�Ŀͻ��˵ĵ�ַ��
* �ṹ�����ã��������������Ͽͻ���ʱ����Ϣ�洢���ýṹ���У�֪���ͻ��˵ĵ�ַ�Ա��ڻطá�
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