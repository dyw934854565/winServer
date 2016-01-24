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

	//不同的文件后缀，返回不同的contentType
	this->fileExt["ez"] = "application/andrew-inset";
	this->fileExt["hqx"] = "application/mac-binhex40";
	this->fileExt["cpt"] = "application/mac-compactpro";
	this->fileExt["doc"] = "application/msword";
	this->fileExt["bin"] = "application/octet-stream";
	this->fileExt["dms"] = "application/octet-stream";
	this->fileExt["lha"] = "application/octet-stream";
	this->fileExt["lzh"] = "application/octet-stream";
	this->fileExt["exe"] = "application/octet-stream";
	this->fileExt["eml"] = "image/jpeg";
	this->fileExt["chm"] = "application/octet-stream";
	this->fileExt["class"] = "application/octet-stream";
	this->fileExt["so"] = "application/octet-stream";
	this->fileExt["dll"] = "application/octet-stream";
	this->fileExt["oda"] = "application/oda";
	this->fileExt["pdf"] = "application/pdf";
	this->fileExt["ai"] = "application/postscript";
	this->fileExt["eps"] = "application/postscript";
	this->fileExt["ps"] = "application/postscript";
	this->fileExt["smi"] = "application/smil";
	this->fileExt["smil"] = "application/smil";
	this->fileExt["mif"] = "application/vnd.mif";
	this->fileExt["xls"] = "application/vnd.ms-excel";
	this->fileExt["ppt"] = "application/vnd.ms-powerpoint";
	this->fileExt["wbxml"] = "application/vnd.wap.wbxml";
	this->fileExt["wmlc"] = "application/vnd.wap.wmlc";
	this->fileExt["wmlsc"] = "application/vnd.wap.wmlscriptc";
	this->fileExt["bcpio"] = "application/x-bcpio";
	this->fileExt["vcd"] = "application/x-cdlink";
	this->fileExt["pgn"] = "application/x-chess-pgn";
	this->fileExt["cpio"] = "application/x-cpio";
	this->fileExt["csh"] = "application/x-csh";
	this->fileExt["dcr"] = "application/x-director";
	this->fileExt["dir"] = "application/x-director";
	this->fileExt["dxr"] = "application/x-director";
	this->fileExt["dvi"] = "application/x-dvi";
	this->fileExt["spl"] = "application/x-futuresplash";
	this->fileExt["gtar"] = "application/x-gtar";
	this->fileExt["hdf"] = "application/x-hdf";
	this->fileExt["js"] = "application/javascript";
	this->fileExt["skp"] = "application/x-koan";
	this->fileExt["skd"] = "application/x-koan";
	this->fileExt["skt"] = "application/x-koan";
	this->fileExt["skm"] = "application/x-koan";
	this->fileExt["latex"] = "application/x-latex";
	this->fileExt["nc"] = "application/x-netcdf";
	this->fileExt["cdf"] = "application/x-netcdf";
	this->fileExt["sh"] = "application/x-sh";
	this->fileExt["shar"] = "application/x-shar";
	this->fileExt["swf"] = "application/x-shockwave-flash";
	this->fileExt["sit"] = "application/x-stuffit";
	this->fileExt["sv4cpio"] = "application/x-sv4cpio";
	this->fileExt["sv4crc"] = "application/x-sv4crc";
	this->fileExt["tar"] = "application/x-tar";
	this->fileExt["tcl"] = "application/x-tcl";
	this->fileExt["tex"] = "application/x-tex";
	this->fileExt["texinfo"] = "application/x-texinfo";
	this->fileExt["texi"] = "application/x-texinfo";
	this->fileExt["t"] = "application/x-troff";
	this->fileExt["tr"] = "application/x-troff";
	this->fileExt["roff"] = "application/x-troff";
	this->fileExt["man"] = "application/x-troff-man";
	this->fileExt["me"] = "application/x-troff-me";
	this->fileExt["ms"] = "application/x-troff-ms";
	this->fileExt["ustar"] = "application/x-ustar";
	this->fileExt["src"] = "application/x-wais-source";
	this->fileExt["xhtml"] = "application/xhtml+xml";
	this->fileExt["xht"] = "application/xhtml+xml";
	this->fileExt["zip"] = "application/zip";
	this->fileExt["rar"] = "application/octet-stream";
	this->fileExt["au"] = "audio/basic";
	this->fileExt["snd"] = "audio/basic";
	this->fileExt["mid"] = "audio/midi";
	this->fileExt["midi"] = "audio/midi";
	this->fileExt["kar"] = "audio/midi";
	this->fileExt["mpga"] = "audio/mpeg";
	this->fileExt["mp2"] = "audio/mpeg";
	this->fileExt["mp3"] = "audio/mpeg";
	this->fileExt["aif"] = "audio/x-aiff";
	this->fileExt["aiff"] = "audio/x-aiff";
	this->fileExt["aifc"] = "audio/x-aiff";
	this->fileExt["m3u"] = "audio/x-mpegurl";
	this->fileExt["ram"] = "audio/x-pn-realaudio";
	this->fileExt["rm"] = "audio/x-pn-realaudio";
	this->fileExt["rpm"] = "audio/x-pn-realaudio-plugin";
	this->fileExt["ra"] = "audio/x-realaudio";
	this->fileExt["wav"] = "audio/x-wav";
	this->fileExt["pdb"] = "chemical/x-pdb";
	this->fileExt["xyz"] = "chemical/x-xyz";
	this->fileExt["bmp"] = "image/bmp";
	this->fileExt["gif"] = "image/gif";
	this->fileExt["ief"] = "image/ief";
	this->fileExt["jpeg"] = "image/jpeg";
	this->fileExt["jpg"] = "image/jpeg";
	this->fileExt["jpe"] = "image/jpeg";
	this->fileExt["png"] = "image/png";
	this->fileExt["tiff"] = "image/tiff";
	this->fileExt["tif"] = "image/tiff";
	this->fileExt["djvu"] = "image/vnd.djvu";
	this->fileExt["djv"] = "image/vnd.djvu";
	this->fileExt["wbmp"] = "image/vnd.wap.wbmp";
	this->fileExt["ras"] = "image/x-cmu-raster";
	this->fileExt["pnm"] = "image/x-portable-anymap";
	this->fileExt["pbm"] = "image/x-portable-bitmap";
	this->fileExt["pgm"] = "image/x-portable-graymap";
	this->fileExt["ppm"] = "image/x-portable-pixmap";
	this->fileExt["rgb"] = "image/x-rgb";
	this->fileExt["xbm"] = "image/x-xbitmap";
	this->fileExt["xpm"] = "image/x-xpixmap";
	this->fileExt["xwd"] = "image/x-xwindowdump";
	this->fileExt["igs"] = "model/iges";
	this->fileExt["iges"] = "model/iges";
	this->fileExt["msh"] = "model/mesh";
	this->fileExt["mesh"] = "model/mesh";
	this->fileExt["silo"] = "model/mesh";
	this->fileExt["wrl"] = "model/vrml";
	this->fileExt["vrml"] = "model/vrml";
	this->fileExt["css"] = "text/css";
	this->fileExt["html"] = "text/html";
	this->fileExt["htm"] = "text/html";
	this->fileExt["asc"] = "text/plain";
	this->fileExt["txt"] = "text/plain";
	this->fileExt["rtx"] = "text/richtext";
	this->fileExt["rtf"] = "text/rtf";
	this->fileExt["sgml"] = "text/sgml";
	this->fileExt["sgm"] = "text/sgml";
	this->fileExt["tsv"] = "text/tab-separated-values";
	this->fileExt["wml"] = "text/vnd.wap.wml";
	this->fileExt["wmls"] = "text/vnd.wap.wmlscript";
	this->fileExt["etx"] = "text/x-setext";
	this->fileExt["xsl"] = "text/xml";
	this->fileExt["xml"] = "text/xml";
	this->fileExt["mpeg"] = "video/mpeg";
	this->fileExt["mpg"] = "video/mpeg";
	this->fileExt["mpe"] = "video/mpeg";
	this->fileExt["qt"] = "video/quicktime";
	this->fileExt["mov"] = "video/quicktime";
	this->fileExt["mxu"] = "video/vnd.mpegurl";
	this->fileExt["avi"] = "video/x-msvideo";
	this->fileExt["movie"] = "video/x-sgi-movie";
	this->fileExt["ice"] = "x-conference/x-cooltalk";
	this->fileExt["json"] = "application/json";

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
	MultiByteToWideChar(CP_UTF8, 0, url.c_str(), url.length() + 1, relPath, PATH_SIZE);
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
		HANDLE findHandle = FindFirstFile(fullPath, &findData);
		FindClose(findHandle);
		//判断文件后缀，返回不同的contenttype
		char temp[PATH_SIZE];
		WideCharToMultiByte(CP_ACP, NULL, fullPath, wcslen(fullPath) + 1, temp, PATH_SIZE, NULL, NULL);
		string str(temp);
		size_t index = str.find_last_of('.');
		string fileExt = str.substr(index+1);
		
		cout << fileExt << endl;
		cout << this->fileExt[fileExt] << endl;
		res.setHeader("Content-Type", this->fileExt[fileExt]);
		if (!findData.nFileSizeHigh)
		{
			string header = res.getHeaderStr(this->serverStr, findData.nFileSizeLow);
			
			ZeroMemory(PerIoData, sizeof(PER_IO_DATA));
			sprintf_s(PerIoData->buffer, DataBuffSize, "%s", header.c_str());

			if (findData.nFileSizeLow == 0) //文件长度为0
			{
				PerIoData->NeedRecv = true;
			}
			else
			{
				PerIoData->ClientHandle = CreateFile(fullPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
				LPPER_HANDLE_DATA newHandleData = new PER_HANDLE_DATA();
				newHandleData->handle = PerIoData->ClientHandle;
				newHandleData->fileLength = findData.nFileSizeLow;
				newHandleData->fileReaded = 0;
				//为文件操作绑定完成端口
				CreateIoCompletionPort(PerIoData->ClientHandle, this->getCompletionPort(), (DWORD)newHandleData, 0);
				if (findData.nFileSizeLow > DataBuffSize)
				{
					PerIoData->length = DataBuffSize;
				}
				else
				{
					PerIoData->length = findData.nFileSizeLow;
				}
				PerIoData->overlapped = { 0 };
				PerIoData->overlapped.Offset = 0;
				PerIoData->NeedRecv = false;
			}
			this->send(clientSock, PerIoData);
		}
		return false;
	}
	else if (access && !isFile) //路径是个目录，返回目录下文件
	{
		HANDLE findHandle;
		wstring body;
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
		char bodyChar[DataBuffSize];
		WideCharToMultiByte(CP_UTF8, NULL, body.c_str(), body.length() + 1, bodyChar, DataBuffSize, NULL, NULL);
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
		res.setHeader("Content-Type", "text/html;");
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
		DWORD length = PerIoData->length;
		DWORD offset = PerIoData->overlapped.Offset;
		ZeroMemory(PerIoData, sizeof(PER_IO_DATA));
		PerIoData->operationType = FILE_READ;
		PerIoData->ClientHandle = (HANDLE)clientSocket;
		PerIoData->length = length;
		PerIoData->overlapped = {0};
		PerIoData->overlapped.Offset = offset;
		ReadFile(hFile, PerIoData->buffer, length, NULL, &PerIoData->overlapped);
		//cout << GetLastError() << endl;
	}
	return PerIoData->NeedRecv;
}
void Http::OnComplite(int type, LPPER_HANDLE_DATA PerHandleData, LPPER_IO_DATA PerIoData, DWORD BytesTransferred)
{
	if (type == FILE_READ)
	{
		BOOL readOut = false;
		PerHandleData->fileReaded += PerIoData->length;
		//cout << BytesTransferred << "-" << PerIoData->overlapped.Offset << endl;
		SOCKET clientSock = (SOCKET)PerIoData->ClientHandle;
		//cout << PerHandleData->fileLength << "-" << PerHandleData->fileReaded << "-" << PerIoData->length << endl;
		//PerIoData->buffer[BytesTransferred] = '\0';
		//cout << strlen(PerIoData->buffer) << endl;
		/*********读文件时候不能用strlen判断长度 **************/
		if (PerHandleData->fileLength == PerHandleData->fileReaded)
		{
			readOut = true;
			//已经读完，关闭文件
			CloseHandle(PerHandleData->handle);
			delete PerHandleData;
			PerHandleData = NULL;
			PerIoData->NeedRecv = true;
		}
		else
		{
			PerIoData->NeedRecv = false;
			//处理读文件位置，异步读文件操作不能用setFilePointer
			PerIoData->overlapped.Offset += PerIoData->length;
		}
		if (!BytesTransferred)
		{
			this->recv(clientSock, PerIoData);
			return;
		}
		if (!readOut)
		{
			//没有读完，把文件句柄传给发送操作，以供下次读文件操作
			PerIoData->ClientHandle = PerHandleData->handle;
			if ((PerHandleData->fileLength - PerHandleData->fileReaded) > DataBuffSize)
			{
				PerIoData->length = DataBuffSize;
			}
			else
			{
				PerIoData->length = PerHandleData->fileLength - PerHandleData->fileReaded;
			}
		}
		this->send(clientSock, PerIoData, BytesTransferred);
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
	//暂时用不上，就不写了
	return str;
}
void Http::clientHandler()
{
	//todo,处理连接中的客户端，5s没发数据,或发送次数等于100的，断开连接。客户端先断开的连接清理数据
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
			sDest += (char)(16 * sSource[nLength + 1] + sSource[nLength + 2]);
			nLength += 2;
			continue;
		}
		sDest += sSource[nLength];
	}
	return sDest;
}
Http::~Http()
{
	this->statucodes.empty();
	this->fileExt.empty();
}