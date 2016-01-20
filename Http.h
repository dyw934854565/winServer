#pragma once
#include "TcpServer.h"
#include "ServerConfig.h"
#include "ReqHeader.h"
#include "ResHeader.h"
class Http : TcpServer
{
public:
	Http();
	BOOL OnRecv(SOCKET clientSock, LPPER_IO_DATA PerIoData);
	BOOL OnSend(SOCKET clientSock, LPPER_IO_DATA PerIoData);
	~Http();
private:
	Config* config;
	string serverStr = "winServer by dyw";
	map <string, string> statucodes;
};

