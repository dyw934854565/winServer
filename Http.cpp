#include "stdafx.h"
#include "Http.h"

Http::Http()
{
	this->config = new Config(TEXT("\\server.ini"));
	this->port = this->config->getPort();

	//返回状态码和信息
	this->statucodes["100"] = "Continue";
	this->statucodes["101"] = "Switching protocols";
	this->statucodes["200"] = "OK";
	this->statucodes["201"] = "Created";
	this->statucodes["202"] = "Accepted";
	this->statucodes["203"] = "Non-Authoritative Information";
	this->statucodes["204"] = "No Content";
	this->statucodes["205"] = "Reset Content";
	this->statucodes["206"] = "Partial Content";
	this->statucodes["300"] = "Multiple Choices";
	this->statucodes["301"] = "Moved Permanently";
	this->statucodes["302"] = "Found";
	this->statucodes["303"] = "See Other";
	this->statucodes["304"] = "Not Modified";
	this->statucodes["305"] = "Use Proxy";
	this->statucodes["307"] = "Temporary Redirect";
	this->statucodes["400"] = "Bad Request";
	this->statucodes["401"] = "Unauthorized";
	this->statucodes["402"] = "Payment Required";
	this->statucodes["403"] = "Forbidden";
	this->statucodes["404"] = "Not Found";
	this->statucodes["405"] = "Method Not Allowed";
	this->statucodes["406"] = "Not Acceptable";
	this->statucodes["407"] = "Proxy Authentication Required";
	this->statucodes["408"] = "Request Timeout";
	this->statucodes["409"] = "Conflict";
	this->statucodes["410"] = "Gone";
	this->statucodes["411"] = "Length Required";
	this->statucodes["412"] = "Precondition Failed";
	this->statucodes["413"] = "Request Entity Too Large";
	this->statucodes["414"] = "Request-URI Too Long";
	this->statucodes["415"] = "Unsupported Media Type";
	this->statucodes["416"] = "Requested Range Not Suitable";
	this->statucodes["417"] = "Expectation Failed";
	this->statucodes["500"] = "Internal Server Error";
	this->statucodes["501"] = "Not Implemented";
	this->statucodes["502"] = "Bad Gateway";
	this->statucodes["503"] = "Service Unavailable";
	this->statucodes["504"] = "Gateway Timeout";
	this->statucodes["505"] = "HTTP Version Not Supported";


	this->CreatServer();
}
BOOL Http::OnRecv(SOCKET clientSock, LPPER_IO_DATA PerIoData)
{
	ReqHeader req = ReqHeader(PerIoData->databuff.buf);
	ResHeader res = ResHeader();
	cout << req.getResult() << endl;
	string url;
	TCHAR relPath[PATH_SIZE] = TEXT("");
	TCHAR fullPath[PATH_SIZE] = TEXT("");
	TCHAR fullPathWithPage[PATH_SIZE] = TEXT("");
	size_t pos = req.getUrl().find("?");
	if (pos > 0)
	{
		url = req.getUrl().substr(0, pos);
	}
	else
	{
		url = req.getUrl();
	}
	MultiByteToWideChar(CP_ACP, 0, url.c_str(), url.length(), relPath, PATH_SIZE);
	wsprintfW(fullPath, TEXT("%s%s"), config->getDocumentRoot(), relPath);
	GetFullPathName(fullPath, PATH_SIZE, fullPathWithPage, NULL);
	wcout << fullPathWithPage << endl;
	BOOL access = true;
	BOOL isFile = true;  //文件或目录
	WIN32_FIND_DATA findData;
	if (-1 == _waccess(fullPathWithPage, 4)) //文件不存在
	{
		access = false;
	}
	if (access)
	{
		HANDLE findHandle = FindFirstFile(fullPathWithPage, &findData);
		cout << findData.dwFileAttributes << FILE_ATTRIBUTE_DIRECTORY << endl;
		if (findHandle != INVALID_HANDLE_VALUE) //是个文件
		{
		}
		else
		{
			isFile = false;
		}
		FindClose(findHandle);
	}
	if (access && !isFile) //路径是一个存在的目录 ，就去查找默认首页
	{
		wsprintfW(fullPath, TEXT("%s%s"), fullPathWithPage, config->getDefaultPage());
		GetFullPathName(fullPath, PATH_SIZE, fullPathWithPage, NULL);
		cout << fullPathWithPage << endl;
		if (-1 == _waccess(fullPathWithPage, 4))
		{
			isFile = false;
		}
		else
		{
			access = true;
			HANDLE findHandle = FindFirstFile(fullPathWithPage, &findData);
			FindClose(findHandle);
		}
	}
	if (access && isFile) //路径文件或者默认首页
	{
		cout << "file" << endl;
	}
	else if (access && !isFile) //路径是个目录，返回目录下文件
	{
		cout << "directory" << endl;
	}
	else //路径找不到，或者是个目录但是没有默认文件，返回404
	{
		res.setStatusCode(string("404 ") + this->statucodes["404"]);
		string body = "<h1 style=\"text-align: center\">not found</h1>";
		string response = res.getHeaderStr(this->serverStr, body.length());
		ZeroMemory(PerIoData, sizeof(PER_IO_DATA));

		sprintf_s(PerIoData->buffer, DataBuffSize, "%s%s", response.c_str(), body.c_str());
		PerIoData->databuff.buf = PerIoData->buffer;
		PerIoData->NeedRecv = true;
		this->send(clientSock, PerIoData);
		return false;
	}
	
	return true;
}
BOOL Http::OnSend(SOCKET clientSocket, LPPER_IO_DATA PerIoData)
{
	return PerIoData->NeedRecv;
}
Http::~Http()
{
}