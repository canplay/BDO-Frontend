#pragma once

#define _WINSOCKAPI_
#include <string>
#include <vector>
#include <map>
#include <io.h>

namespace HL
{
	class Windows
	{
	public:
		struct _HLFILEINFO
		{
			char name[260];
			char version[260];
			int size;
			char date[260];
		};

		struct _TRAYICONINFO
		{
			void* hWnd = 0;
			const char* szTitle = 0;
			const char* szIcon = 0;
		};

	public:
		static bool CheckFileExist(wchar_t* szPath);
		static bool DelFile(wchar_t* szPath);

		static bool GetAppPath(wchar_t* szPath);
		static bool GetAppName(wchar_t* szName);
		static bool GetDesktopPath(wchar_t* szDesktopPath);
		static bool GetIEQuickLaunchPath(wchar_t* szPath);
		static bool GetProgramsPath(wchar_t* szPath);
		static bool GetStartupPath(wchar_t* szPath);
		static bool GetSystemPath(wchar_t* szPath, bool x86 = true);
		static bool GetProgramFilesPath(wchar_t* szPath, bool x86 = true);

		static bool CreateFileShortcut(const char* szFileName, const char* szLnkFileDir, const char* szLnkFileName, const char* szWorkDir, unsigned short wHotkey, const char* szDescription, int iShowCmd = 1);
		
		static void* GetCurrentModule(bool bRef = false);

		static bool SearchFile(const char* szFolder, const char* szFilter, std::vector<_HLFILEINFO*>* fileList);

		static void CenterWindow(void* hWnd);

		static bool isStarted(const char* szClass);

		static int AddTrayIcon(_TRAYICONINFO icon);
		static void SetTrayIcon(int nID, _TRAYICONINFO icon);
		static void DelTrayIcon(int nID = -1);

		static bool ShellExe(const char* szFile, const char* szPara = "", bool bShow = true, bool bWait = false);

		static int KillProcess(const char* szFilename, const char* szDir = 0);
		static int GetProcessFullPath(unsigned long dwPID, char pszFullPath[260]);

		static int Random(int range);

		static bool ExtractZip(const char* szFile, const char* szPath);

	private:
		static void Kill(int pid);
		static int DosPathToNtPath(char* pszDosPath, char* pszNtPath);
	};
}
