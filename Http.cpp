#include "stdafx.h"
#include "Http.h"

Http::Http()
{
	this->config = new Config(TEXT("\\server.ini"));
	this->port = this->config->getPort();
	this->CreatServer();
}
BOOL Http::OnRecv(SOCKET clientSock, LPPER_IO_DATA PerIoData)
{
	ReqHeader* req = new ReqHeader(PerIoData->databuff.buf);
	return true;
}
Http::~Http()
{
}