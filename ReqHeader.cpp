#include "stdafx.h"
#include "ReqHeader.h"

ReqHeader::ReqHeader(char* reqstr)
{
	string str(reqstr);
	size_t headerSize = str.find("\r\n\r\n");
	this->header = str.substr(0, headerSize);
	this->body = str.substr(headerSize+4);

	cout << endl << string("\r\n\r\n").length() << "   " << str.max_size()<< endl;
	cout << this->body << endl << "·Ö¸îÏß" << endl << this->header;
	cout << endl << this->body.length() << " " << this->header.length() << " " << str.length() << endl;

	size_t index = 0;
	while (true)
	{
		size_t tempIndex = index;
		string header;
		index = this->header.find("\r\n", index);
		if (index > 0)
		{
			if (tempIndex == 0)
			{

			}
			else
			{
				
			}
			
		}
		else
		{
			
		}
		
	}

	char* temp = NULL;
	char* token = strtok_s(reqstr, "\r\n", &temp);
	while (token != NULL)
	{
		/* While there are tokens in "string" */
		printf("%s ", token);
		/* Get next token: */
		token = strtok_s(NULL, "\r\n", &temp);
	}
}
string ReqHeader::getResult()
{
	return this->result;
}
ReqHeader::~ReqHeader()
{
}