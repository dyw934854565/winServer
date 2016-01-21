#include "stdafx.h"
#include "ServerConfig.h"
Config::Config(TCHAR* path)
{
	DWORD dwRet;
	dwRet = GetCurrentDirectory(PATH_SIZE, this->ServerRoot);
	if (dwRet == 0)
	{
		_tprintf(TEXT("GetCurrentDirectory failed (%d)\n"), GetLastError());
	}
	else if (dwRet > PATH_SIZE)
	{
		_tprintf(TEXT("GetCurrentDirectory failed (buffer too small; need %d chars)\n"), dwRet);
	}
	else
	{
		
		TCHAR bufferpath[1000];
		wsprintfW(bufferpath, TEXT("%s%s"), this->ServerRoot, path);

		//set default setting
		wsprintfW(this->DocumentRoot, TEXT("%s%s"), this->ServerRoot, TEXT("\\www"));
		if (-1 != _waccess(bufferpath, 4))
		{
			//get Port
			this->Port = GetPrivateProfileInt(TEXT("server"), TEXT("port"), this->getPort(), bufferpath);

			//get document root
			GetPrivateProfileString(TEXT("server"), TEXT("DocumentRoot"), this->DocumentRoot, this->DocumentRoot, PATH_SIZE, bufferpath);

			//get default page
			GetPrivateProfileString(TEXT("server"), TEXT("DefaultPage"), TEXT("index.html"), this->DefaultPage, PATH_SIZE, bufferpath);
		}
		else
		{
			cout << "can'not find the server.ini config file, server is created with default setting" << endl;
		}
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
TCHAR* Config::getDocumentRoot()
{
	return this->DocumentRoot;
}
TCHAR* Config::getServerRoot()
{
	return this->ServerRoot;
}
TCHAR* Config::getDefaultPage()
{
	return this->DefaultPage;
}