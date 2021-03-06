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
	char buffer[DataBuffSize + 1];  //+1保证读文件字符串有正常的结尾 '\0'
	DWORD length;
	int operationType;
	BOOL NeedRecv;
	//DWORD Offset;
	//DWORD OffsetHigh;
	//HANDLE hEvent;
}PER_IO_OPERATEION_DATA, *LPPER_IO_OPERATION_DATA, *LPPER_IO_DATA, PER_IO_DATA;

/**
* 结构体名称：PER_HANDLE_DATA
* 结构体存储：记录单个套接字的数据，包括了套接字的变量及套接字的对应的客户端的地址。
* 结构体作用：当服务器连接上客户端时，信息存储到该结构体中，知道客户端的地址以便于回访。
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
