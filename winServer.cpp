// winServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ServerConfig.h"
#include "Server.h"

int main()
{
	Config* config = new Config(TEXT("\\server.ini"));
	
	Server* server = new Server(config);
	getchar();
	getchar();
    return 0;
}

