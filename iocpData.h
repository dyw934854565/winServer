#pragma once
#include "stdafx.h"

#define OP_READ    1
#define OP_WRITE   2
#define OP_ACCEPT  3
#define FILE_READ  4
#define FILE_WRITE 5

#define DataBuffSize 1024*1024

typedef struct
{
	HANDLE ClientHandle;
	OVERLAPPED overlapped;
	WSABUF databuff;
	char buffer[DataBuffSize];
	DWORD length;
	int operationType;
	BOOL NeedRecv;
}PER_IO_OPERATEION_DATA, *LPPER_IO_OPERATION_DATA, *LPPER_IO_DATA, PER_IO_DATA;

/**
* �ṹ�����ƣ�PER_HANDLE_DATA
* �ṹ��洢����¼�����׽��ֵ����ݣ��������׽��ֵı������׽��ֵĶ�Ӧ�Ŀͻ��˵ĵ�ַ��
* �ṹ�����ã��������������Ͽͻ���ʱ����Ϣ�洢���ýṹ���У�֪���ͻ��˵ĵ�ַ�Ա��ڻطá�
**/
typedef struct
{
	HANDLE handle;
	SOCKADDR_STORAGE addr;
	SYSTEMTIME lastRecv;
	SYSTEMTIME lastSend;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;
