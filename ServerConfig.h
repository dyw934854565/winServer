#pragma once
class Config
{
public:
	Config(TCHAR* path);
	Config();
	~Config();
	int getPort();
	void setPort(int port);

private:
	int port = 80;
};