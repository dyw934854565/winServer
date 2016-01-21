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
	string url;
	TCHAR relPath[PATH_SIZE] = TEXT("");
	TCHAR pathTemp[PATH_SIZE] = TEXT("");
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
	url = this->decodeURI(url);
	//cout << url << endl;
	MultiByteToWideChar(CP_UTF8, 0, url.c_str(), url.length(), relPath, PATH_SIZE);
	wsprintfW(pathTemp, TEXT("%s%s"), config->getDocumentRoot(), relPath);
	GetFullPathName(pathTemp, PATH_SIZE, fullPath, NULL);
	BOOL access = true;
	BOOL isFile = true;  //文件或目录
	WIN32_FIND_DATA findData;
	if (-1 == _waccess(fullPath, 4)) //文件不存在
	{
		access = false;
	}
	if (access)
	{
		HANDLE findHandle = FindFirstFile(fullPath, &findData);
		FindClose(findHandle);
		if (findHandle != INVALID_HANDLE_VALUE) //是个文件
		{
		}
		else
		{
			isFile = false;
		}
	}
	if (access && !isFile) //路径是一个存在的目录 ，就去查找默认首页
	{
		wsprintfW(pathTemp, TEXT("%s\\%s"), fullPath, config->getDefaultPage());
		GetFullPathName(pathTemp, PATH_SIZE, fullPathWithPage, NULL);
		if (-1 == _waccess(fullPathWithPage, 4))
		{
			isFile = false;
		}
		else
		{
			wsprintfW(fullPath, TEXT("%s"), fullPathWithPage);
			isFile = true;
		}
	}
	if (access && isFile) //路径文件或者默认首页
	{
		HANDLE findHandle = FindFirstFile(fullPathWithPage, &findData);
		FindClose(findHandle);

		//todo, 判断文件后缀，返回不同的contenttype


		if (!findData.nFileSizeHigh)
		{
			string header = res.getHeaderStr(this->serverStr, findData.nFileSizeLow);
			
			ZeroMemory(PerIoData, sizeof(PER_IO_DATA));
			sprintf_s(PerIoData->buffer, DataBuffSize, "%s", header.c_str());
			PerIoData->ClientHandle = CreateFile(fullPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
			
			LPPER_HANDLE_DATA newHandleData = new PER_HANDLE_DATA();
			newHandleData->handle = PerIoData->ClientHandle;
			//为文件操作绑定完成端口
			CreateIoCompletionPort(PerIoData->ClientHandle, this->getCompletionPort(), (DWORD)newHandleData, 0);
			PerIoData->length = 0;
			PerIoData->NeedRecv = false;
			this->send(clientSock, PerIoData);
		}
		return false;
	}
	else if (access && !isFile) //路径是个目录，返回目录下文件
	{
		HANDLE findHandle;
		wstring body;
		char bodyChar[1024];
		body += TEXT("<html>\r\n<head><title>Index Of/</title></head>\r\n<body>\r\n");
		body += TEXT("<h1>Index Of/</h1>\r\n");
		wsprintfW(pathTemp, TEXT("%s\\*"), fullPath);
		
		findHandle = FindFirstFile(pathTemp, &findData);
		if (findHandle != INVALID_HANDLE_VALUE)
		{
			while (TRUE)
			{
				if (findData.cFileName[0] == TEXT('.'))
				{

				}
				else
				{
					if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						TCHAR item[200];
						wsprintfW(item, TEXT("<p><a href=\"%s/\">%s</a></p>\r\n"), findData.cFileName, findData.cFileName);
						body += item;
					}
					else
					{
						TCHAR item[200];
						wsprintfW(item, TEXT("<p><a href=\"%s\">%s</a></p>\r\n"), findData.cFileName, findData.cFileName);
						body += item;
					}
				}
				FindNextFile(findHandle, &findData);
				if (GetLastError() == ERROR_NO_MORE_FILES)
				{
					break;
				}
			}
		}
		FindClose(findHandle);
		body += TEXT("</body>\r\n</html>");
		WideCharToMultiByte(CP_UTF8, NULL, body.c_str(), string::npos, bodyChar, 1024, NULL, NULL);
		res.setHeader("Content-Type", "text/html;charset=UTF-8");
		string response = res.getHeaderStr(this->serverStr, strlen(bodyChar));
		ZeroMemory(PerIoData, sizeof(PER_IO_DATA));

		sprintf_s(PerIoData->buffer, DataBuffSize, "%s%s", response.c_str(), bodyChar);
		PerIoData->NeedRecv = true;
		this->send(clientSock, PerIoData);
		return false;
	}
	else //路径找不到，或者是个目录但是没有默认文件，返回404
	{
		res.setStatusCode(string("404 ") + this->statucodes["404"]);
		string body = "<h1 style=\"text-align: center\">not found</h1>";
		res.setHeader("Content-Type", "text/html;charset=UTF-8");
		string response = res.getHeaderStr(this->serverStr, body.length());
		ZeroMemory(PerIoData, sizeof(PER_IO_DATA));

		sprintf_s(PerIoData->buffer, DataBuffSize, "%s%s", response.c_str(), body.c_str());
		PerIoData->NeedRecv = true;
		this->send(clientSock, PerIoData);
		return false;
	}
	
	return true;
}
BOOL Http::OnSend(SOCKET clientSocket, LPPER_IO_DATA PerIoData)
{
	if (!PerIoData->NeedRecv)//如果是false,说明没有发送完。继续发送文件
	{
		HANDLE hFile = PerIoData->ClientHandle;
		ZeroMemory(PerIoData, sizeof(PER_IO_DATA));
		PerIoData->operationType = FILE_READ;
		PerIoData->ClientHandle = (HANDLE)clientSocket;
		ReadFile(hFile, PerIoData->buffer, DataBuffSize, &PerIoData->length, &PerIoData->overlapped);
	}
	return PerIoData->NeedRecv;
}
void Http::OnComplite(int type, LPPER_HANDLE_DATA PerHandleData, LPPER_IO_DATA PerIoData)
{
	if (type == FILE_READ)
	{
		SOCKET clientSock = (SOCKET)PerIoData->ClientHandle;
		DWORD length = strlen(PerIoData->buffer);
		if (length < DataBuffSize)
		{
			//已经读完，关闭文件
			CloseHandle(PerHandleData->handle);
			delete PerHandleData;
			PerHandleData = NULL;
			PerIoData->NeedRecv = true;
		}
		else
		{
			PerIoData->NeedRecv = false;
		}
		if (length > 0)
		{
			//把文件句柄传给发送操作， 以供下次多文件操作
			if (!PerIoData->NeedRecv)
			{
				PerIoData->ClientHandle = PerHandleData->handle;
			}
			this->send(clientSock, PerIoData);
		}
		else if (length == 0)
		{
			this->recv(clientSock, PerIoData);
		}
	}
}
BOOL Http::OnAccept(SOCKET clientSock, LPPER_IO_DATA PerIoData)
{
	return true;
}
BOOL Http::OnClose(LPPER_HANDLE_DATA PerHandleData)
{
	return true;
}
string Http::encodeURI(string str)
{
	return str;
}
string Http::decodeURI(string sSource)
{
	string sDest;
	int nLength;
	for (nLength = 0; nLength < sSource.length(); nLength++) 
	{
		if (sSource[nLength] == '%' && sSource[nLength + 1] && sSource[nLength + 2] && isxdigit(sSource[nLength + 1]) && isxdigit(sSource[nLength + 2])) {
			sSource[nLength + 1] -= sSource[nLength + 1] <= '9' ? '0' : (sSource[nLength + 1] <= 'F' ? 'A' : 'a') - 10;
			sSource[nLength + 2] -= sSource[nLength + 2] <= '9' ? '0' : (sSource[nLength + 2] <= 'F' ? 'A' : 'a') - 10;
			sDest += (16 * sSource[nLength + 1] + sSource[nLength + 2]);
			nLength += 3;
			continue;
		}
		sDest += sSource[nLength];
	}
	return sDest;
}
Http::~Http()
{
}