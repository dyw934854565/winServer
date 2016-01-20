#pragma once
#include <map>
class ReqHeader
{
public:
	ReqHeader(char* reqstr);
	~ReqHeader();
	string getResult();
	string getUrl();
	string getHttpVersion();
	string getMethod();
	string getHeader(string name);
	BOOL parseHeader(string str);
private:
	string header;
	string body;
	string url;
	string httpVersion;
	string method;
	map <string, string> headers;

	string result;
};