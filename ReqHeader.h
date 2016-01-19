#pragma once
#include <map>
class ReqHeader
{
public:
	ReqHeader(char* reqstr);
	~ReqHeader();
	string getResult();
private:
	string header;
	string body;
	string url;
	string httpVersion;
	string method;
	map <string, string> headers;

	string result;
};