#include "stdafx.h"
#include "ReqHeader.h"

ReqHeader::ReqHeader(char* reqstr)
{
	string str(reqstr);
	size_t headerSize = str.find("\r\n\r\n");
	if (headerSize == string::npos)
	{
		cout << str << endl;
		cout << "bad request" << endl;
		this->result = "404";
		return;
	}
	this->header = str.substr(0, headerSize);
	if (str.length() - this->header.length() > 4)
	{
		this->body = str.substr(headerSize + 4);
	}

	size_t index = 0;
	while (true)
	{
		size_t tempIndex = index;
		string header;
		index = this->header.find("\r\n", index);
		if (index < string::npos)
		{
			header = this->header.substr(tempIndex, index - tempIndex);
			if (tempIndex == 0)
			{
				size_t one = 0,tempone=0;
				one = header.find(' ', 0);
				if (one == string::npos)
				{
					cout << "method err" << endl;
					this->result = "400";
					return;
				}
				else
				{
					this->method = header.substr(tempone, one);
					one++;
				}
				tempone = one;
				one = header.find(' ', tempone);
				if (one == string::npos)
				{
					this->result = "400";
					return;
				}
				else
				{
					this->url = header.substr(tempone, one - tempone);
					one++;
				}
				this->httpVersion = header.substr(one);
			}
			else
			{
				if (this->parseHeader(header)) return;
			}
			index += 2;
		}
		else
		{
			header = this->header.substr(tempIndex);
			if (this->parseHeader(header)) return;
			break;
		}
	}
}
string ReqHeader::getResult()
{
	return this->result;
}
string ReqHeader::getUrl()
{
	return this->url;
}
string ReqHeader::getHttpVersion()
{

	return this->httpVersion;
}
string ReqHeader::getMethod()
{
	return this->method;
}
BOOL ReqHeader::parseHeader(string str)
{
	size_t pos = str.find(": ");
	if (pos < string::npos)
	{
		this->headers[str.substr(0, pos)] = str.substr(pos + 2);
	}
	else
	{
		cout << "header err" << endl;
		this->result = "400";
		return true;
	}
	return false;
}
string ReqHeader::getHeader(string name)
{

	return "aa";
}
ReqHeader::~ReqHeader()
{
}