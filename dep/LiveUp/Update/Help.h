#pragma once

#include <string>
#include <vector>

class Help
{
public:
	struct _HLFILEINFO
	{
		char name[260];
		char version[260];
		int size;
		char date[260];
	};

public:
	Help();
	virtual ~Help();

	static Help& GetInstance();

	bool isStarted(const wchar_t* szClass);

	bool KillProcess(const wchar_t* szFilename, const wchar_t* szDir = 0);
	int GetProcessFullPath(unsigned long dwPID, wchar_t pszFullPath[260]);

	bool GetAppPath(wchar_t* szPath);

	std::string UTF8ToGBK(const std::string& strUTF8);
	std::string GBKToUTF8(const std::string& strGBK);

	std::string WideToMulit(const wchar_t* strWide);
	std::wstring MulitToWide(const char* strMulite);

	bool ShellExe(const wchar_t* szFile);

	bool ExtractZip(const char* szFile, const char* szPath);

	void Log(const char* sFormat, ...);

	bool SearchFile(const char* szFolder, const char* szFilter, std::vector<_HLFILEINFO*> fileList);

private:
	void Kill(int pid);
	int DosPathToNtPath(wchar_t* pszDosPath, wchar_t* pszNtPath);

private:
	FILE* m_pFile;
};

