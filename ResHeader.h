#pragma once
#include "stdafx.h"
class ResHeader
{
public:
	ResHeader(string header);
	~ResHeader();

	int getUTCstr(char* date);

private:
	char statusCode[4] = "200";
	char httpVersion[10] = "HTTP/1.1";
	char statusTip[10] = "OK";
	char Date[100];
	
};