#include "stdafx.h"
#include "ReqHeader.h"

ReqHeader::ReqHeader(char* reqstr)
{
	string str(reqstr);
	size_t headerSize = str.find("\r\n\r\n");
	this->header = str.substr(0, headerSize);
	this->body = str.substr(headerSize+4);

	size_t index = 0;
	while (true)
	{
		size_t tempIndex = index;
		string header;
		index = this->header.find("\r\n", index);
		if (index > 0)
		{
			header = this->header.substr(tempIndex, index - tempIndex);
			if (tempIndex == 0)
			{
				size_t one = 0,tempone=0;
				one = header.find(' ', 0);
				if (one < 0)
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
				if (one < 0)
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
			header = this->header.substr(index);
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
	if (pos > 0)
	{
		this->headers[str.substr(0, pos)] = str.substr(pos + 2);
	}
	else
	{
		cout << "header err" << endl;
		this->result = "400";
	}
	return true;
}
string ReqHeader::getHeader(string name)
{

	return "aa";
}
ReqHeader::~ReqHeader()
{
}