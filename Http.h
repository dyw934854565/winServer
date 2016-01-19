#pragma once
#include "Server.h"
#include "ServerConfig.h"
#include "ReqHeader.h"
#include "ResHeader.h"
class Http : Server
{
public:
	Http();
	BOOL OnRecv(SOCKET clientSock, LPPER_IO_DATA PerIoData);
	~Http();
private:
	Config* config;
};

