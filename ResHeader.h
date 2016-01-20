#pragma once
#include "stdafx.h"
#include <map>
class ResHeader
{
public:
	ResHeader();
	~ResHeader();

	string getUTCstr();
	void setStatusCode(string str);
	void setHttpVersion(string str);
	void setHeader(string name, string value);
	string getHeaderStr(string serverStr, int length = 0);
private:
	string statusCode = "200 OK";
	string httpVersion = "HTTP/1.1";
	map<string, string> resHeaders;
};