#include "stdafx.h"
#include "ServerConfig.h"
Config::Config(TCHAR* path)
{
	DWORD dwRet;
	dwRet = GetCurrentDirectory(BUFFER_SIZE, this->ServerRoot);
	if (dwRet == 0)
	{
		_tprintf(TEXT("GetCurrentDirectory failed (%d)\n"), GetLastError());
	}
	else if (dwRet > BUFFER_SIZE)
	{
		_tprintf(TEXT("GetCurrentDirectory failed (buffer too small; need %d chars)\n"), dwRet);
	}
	else
	{
		TCHAR bufferpath[1000];
		wsprintfW(bufferpath, TEXT("%s%s"), this->ServerRoot, path);

		//get Port
		this->Port = GetPrivateProfileInt(TEXT("server"), TEXT("port"), this->getPort(), bufferpath);

		//get document root
		TCHAR DocumentRootDefault[BUFFER_SIZE];
		wsprintfW(DocumentRootDefault, TEXT("%s%s"), this->ServerRoot, TEXT("\\www"));
		GetPrivateProfileString(TEXT("server"), TEXT("DocumentRoot"), DocumentRootDefault, this->DocumentRoot, BUFFER_SIZE, bufferpath);
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
	return this->Port;
}