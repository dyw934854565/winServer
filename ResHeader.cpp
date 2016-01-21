#include "stdafx.h"
#include "ResHeader.h"

ResHeader::ResHeader()
{
}
ResHeader::~ResHeader()
{
}
void ResHeader::setHeader(string name, string value)
{
	this->resHeaders[name] = value;
}
void ResHeader::setHttpVersion(string str)
{
	this->httpVersion = str;
}
void ResHeader::setStatusCode(string str)
{
	this->statusCode = str;
}
string ResHeader::getUTCstr()
{
	char months[13][4] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	char weeks[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	char date[100];
	SYSTEMTIME now;
	GetSystemTime(&now);
	sprintf_s(date,100, "%s, %02d %s %d %02d:%02d:%02d GMT", weeks[now.wDayOfWeek], now.wDay, months[now.wMonth], now.wYear, now.wHour, now.wMinute, now.wSecond);
	return string(date);
}
string ResHeader::getHeaderStr(string serverStr, DWORD length)
{
	string res(this->httpVersion + ' ' + this->statusCode + "\r\n");
	res += ("Server: " + serverStr + "\r\n");
	res += "Connection: Keep-Alive\r\n";
	res += "Keep-Alive: timeout=5, max=100\r\n";
	if (length)
	{
		char temp[30];
		sprintf_s(temp, 30, "Content-Length: %d\r\n", length);
		res += temp;
	}
	res += ("Date: " + this->getUTCstr() + "\r\n");
	for (auto it = this->resHeaders.cbegin(); it != this->resHeaders.cend(); ++it)
	{
		res += it->first + ": " + it->second + "\r\n";
	}
	res += "\r\n";
	return res;
}