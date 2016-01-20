#pragma once
#define PATH_SIZE 400
class Config
{
public:
	Config(TCHAR* path);
	Config();
	~Config();
	int getPort();
	TCHAR* getDocumentRoot();
	TCHAR* getServerRoot();
	TCHAR* getDefaultPage();

private:

	int Port = 80;

	TCHAR ServerRoot[PATH_SIZE];
	TCHAR DocumentRoot[PATH_SIZE];
	TCHAR DefaultPage[PATH_SIZE] = TEXT("index.html");
};