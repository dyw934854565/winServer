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
	BOOL OnAccept(SOCKET clientSock, LPPER_IO_DATA PerIoData);
	BOOL OnClose(LPPER_HANDLE_DATA PerHandleData);
	void Http::OnComplite(int type, LPPER_HANDLE_DATA PerHandleData, LPPER_IO_DATA PerIoData);

	string encodeURI(string str);
	string decodeURI(string str);
	~Http();
private:
	Config* config;
	string serverStr = "winServer by dyw";
	map <string, string> statucodes;
};

