#include "stdafx.h"
#include "ResHeader.h"

ResHeader::ResHeader(string header)
{
}

ResHeader::~ResHeader()
{
}

int ResHeader::getUTCstr(char* date)
{
	char months[13][4] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	char weeks[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	SYSTEMTIME now;
	GetSystemTime(&now);
	sprintf_s(date,100, "%s, %02d %s %d %02d:%02d:%02d GMT", weeks[now.wDayOfWeek], now.wDay, months[now.wMonth], now.wYear, now.wHour, now.wMinute, now.wSecond);
	return 1;
}