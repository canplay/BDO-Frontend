#pragma once

class __declspec(dllexport) LiveUp
{
public:
	static LiveUp &GetInstance();

public:
	LiveUp();
	virtual ~LiveUp();

	int Init(const char* url1, const char* url2);

	void ShowDlg();

private:
	int CompareVersion(const char* sVer1, const char* sVer2);

	int Check(int nResult, const char* szJson);

	bool ExtractResource();

public:
	HWND m_DlgHWND;

private:
	struct _VERINFO
	{
		int Major;
		int Minor;
		int Revision;
		int Build;
	};

	bool isMandatory;

	wchar_t m_szInfo[MAX_PATH];
	wchar_t m_szOldVersion[MAX_PATH];
	wchar_t m_szNewVersion[MAX_PATH];
};

extern HINSTANCE hInst;

extern "C" __declspec(dllexport) int CheckUpdate(const char* url1, const char* url2);
extern "C" __declspec(dllexport) int UpdateProgress;
