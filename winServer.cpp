// winServer.cpp : �������̨Ӧ�ó������ڵ㡣
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

