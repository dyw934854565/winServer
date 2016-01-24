#pragma once
#include "stdafx.h"

#define OP_READ    1
#define OP_WRITE   2
#define OP_ACCEPT  3
#define FILE_READ  4
#define FILE_WRITE 5

const DWORD DataBuffSize = 1024 * 4;

typedef struct
{
	HANDLE ClientHandle;
	OVERLAPPED overlapped;
	WSABUF databuff;
	char buffer[DataBuffSize + 1];  //+1��֤���ļ��ַ����������Ľ�β '\0'
	DWORD length;
	int operationType;
	BOOL NeedRecv;
	//DWORD Offset;
	//DWORD OffsetHigh;
	//HANDLE hEvent;
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
	DWORD fileLength;
	DWORD fileReaded;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;
