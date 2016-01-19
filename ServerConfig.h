#pragma once
#define BUFFER_SIZE 400
class Config
{
public:
	Config(TCHAR* path);
	Config();
	~Config();
	int getPort();

private:

	int Port = 80;

	TCHAR ServerRoot[BUFFER_SIZE];
	TCHAR DocumentRoot[BUFFER_SIZE];
};