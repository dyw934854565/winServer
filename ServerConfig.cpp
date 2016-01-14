#include "stdafx.h"
#include "ServerConfig.h"
Config::Config(TCHAR* path)
{
	const int BUFFSIZE = 400;
	TCHAR buffer[BUFFSIZE];
	DWORD dwRet;
	dwRet = GetCurrentDirectory(BUFFSIZE, buffer);
	if (dwRet == 0)
	{
		_tprintf(TEXT("GetCurrentDirectory failed (%d)\n"), GetLastError());
	}
	else if (dwRet > BUFFSIZE)
	{
		_tprintf(TEXT("GetCurrentDirectory failed (buffer too small; need %d chars)\n"), dwRet);
	}
	else
	{
		TCHAR bufferpath[1000];
		wsprintfW(bufferpath, TEXT("%s%s"), buffer, path);
		this->port = GetPrivateProfileInt(TEXT("server"), TEXT("port"), this->getPort(), bufferpath);
	}
}
Config::Config()
{
}
Config::~Config()
{
}
int Config::getPort()
{
	return this->port;
}
void Config::setPort(int port)
{
	this->port = port;
}