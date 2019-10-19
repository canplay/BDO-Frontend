#include "HLWindows.h"
#include "HLString.h"

#include <shlobj.h>
#include <stdio.h>
#include <time.h>
#include <direct.h>
#include <shellapi.h>
#include <unzip.h>
#include <re2.h>
#include <tlhelp32.h>
#include <Psapi.h>

namespace HL
{
	static std::vector<_finddata_t> m_FilesInfo;
	static std::map<int, HL::Windows::_TRAYICONINFO> m_TrayIcons;

	bool Windows::CheckFileExist(wchar_t* szPath)
	{
		WIN32_FIND_DATAW wfd;
		HANDLE hFind = FindFirstFileW(szPath, &wfd);
		if (INVALID_HANDLE_VALUE != hFind)
			return true;
		else
			CloseHandle(hFind);
		return false;
	}

	bool Windows::DelFile(wchar_t* szPath)
	{
		WIN32_FIND_DATAW wfd;
		HANDLE hFind = FindFirstFileW(szPath, &wfd);
		if (INVALID_HANDLE_VALUE != hFind)
		{
			::DeleteFileW(szPath);
			return true;
		}
		else
			CloseHandle(hFind);
		return false;
	}

	bool Windows::GetAppPath(wchar_t* szPath)
	{
		GetModuleFileNameW(nullptr, szPath, MAX_PATH);
		(wcsrchr(szPath, _T('\\')))[1] = 0;
		if (szPath != L"") return true;
		return false;
	}

	bool Windows::GetAppName(wchar_t* szName)
	{
		wchar_t szPath[MAX_PATH] = { 0 };
		GetModuleFileNameW(nullptr, szPath, MAX_PATH);
		wchar_t* pbuf = NULL;
		wchar_t* szLine = wcstok_s(szPath, L"\\", &pbuf);
		while (szLine != nullptr)
		{
			wcscpy_s(szName, MAX_PATH, szLine);
			szLine = wcstok_s(NULL, L"\\", &pbuf);
		}
		return true;
	}

	bool Windows::GetDesktopPath(wchar_t* szPath)
	{
		LPITEMIDLIST ppidl = nullptr;

		if (SHGetSpecialFolderLocation(nullptr, CSIDL_DESKTOP, &ppidl) == S_OK)
		{
			BOOL flag = SHGetPathFromIDListW(ppidl, szPath);
			CoTaskMemFree(ppidl);
			return flag;
		}

		return false;
	}

	bool Windows::GetIEQuickLaunchPath(wchar_t* szPath)
	{
		LPITEMIDLIST ppidl = nullptr;

		if (SHGetSpecialFolderLocation(nullptr, CSIDL_APPDATA, &ppidl) == S_OK)
		{
			BOOL flag = SHGetPathFromIDListW(ppidl, szPath);
			wcscat_s(szPath, MAX_PATH, L"\\Microsoft\\Internet Explorer\\Quick Launch");
			CoTaskMemFree(ppidl);
			return flag;
		}

		return false;
	}

	bool Windows::GetProgramsPath(wchar_t* szPath)
	{
		LPITEMIDLIST ppidl = nullptr;

		if (SHGetSpecialFolderLocation(nullptr, CSIDL_PROGRAMS, &ppidl) == S_OK)
		{
			BOOL flag = SHGetPathFromIDListW(ppidl, szPath);
			CoTaskMemFree(ppidl);
			return flag;
		}

		return false;
	}

	bool Windows::GetStartupPath(wchar_t* szPath)
	{
		LPITEMIDLIST ppidl = nullptr;

		if (SHGetSpecialFolderLocation(nullptr, CSIDL_STARTUP, &ppidl) == S_OK)
		{
			BOOL flag = SHGetPathFromIDListW(ppidl, szPath);
			CoTaskMemFree(ppidl);
			return flag;
		}

		return false;
	}

	bool Windows::GetSystemPath(wchar_t* szPath, bool x86)
	{
		LPITEMIDLIST ppidl = nullptr;

		if (x86)
		{
			if (SHGetSpecialFolderLocation(nullptr, CSIDL_SYSTEMX86, &ppidl) == S_OK)
			{
				BOOL flag = SHGetPathFromIDListW(ppidl, szPath);
				CoTaskMemFree(ppidl);
				return flag;
			}
		}
		else
		{
			if (SHGetSpecialFolderLocation(nullptr, CSIDL_SYSTEM, &ppidl) == S_OK)
			{
				BOOL flag = SHGetPathFromIDListW(ppidl, szPath);
				CoTaskMemFree(ppidl);
				return flag;
			}
		}

		return false;
	}

	bool Windows::GetProgramFilesPath(wchar_t* szPath, bool x86)
	{
		LPITEMIDLIST ppidl = nullptr;

		if (x86)
		{
			if (SHGetSpecialFolderLocation(nullptr, CSIDL_PROGRAM_FILESX86, &ppidl) == S_OK)
			{
				BOOL flag = SHGetPathFromIDListW(ppidl, szPath);
				CoTaskMemFree(ppidl);
				return flag;
			}
		}
		else
		{
			if (SHGetSpecialFolderLocation(nullptr, CSIDL_PROGRAM_FILES, &ppidl) == S_OK)
			{
				BOOL flag = SHGetPathFromIDListW(ppidl, szPath);
				CoTaskMemFree(ppidl);
				return flag;
			}
		}

		return false;
	}

	bool Windows::CreateFileShortcut(const char* szFileName, const char* szLnkFileDir, const char* szLnkFileName, const char* szWorkDir, unsigned short wHotkey, const char* szDescription, int iShowCmd)
	{
		if (szLnkFileDir == nullptr)
			return false;

		IShellLink *pLink = nullptr;  //IShellLink对象指针
		IPersistFile *ppf = nullptr; //IPersisFil对象指针

		//创建IShellLink对象
		HRESULT hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pLink);
		if (FAILED(hr)) return false;

		//从IShellLink对象中获取IPersistFile接口
		hr = pLink->QueryInterface(IID_IPersistFile, (void**)&ppf);
		if (FAILED(hr))
		{
			pLink->Release();
			return false;
		}

		//目标
		if (szFileName == nullptr)
		{
			char path[MAX_PATH];
			GetModuleFileName(nullptr, path, MAX_PATH);
			pLink->SetPath(path);
		}
		else
			pLink->SetPath(szFileName);

		//工作目录
		if (szWorkDir != nullptr)
			pLink->SetWorkingDirectory(szWorkDir);
		else
		{
			char workDirBuf[MAX_PATH];
			char path[MAX_PATH];
			GetModuleFileName(nullptr, path, MAX_PATH);

			if (szFileName == nullptr)
				strcpy_s(workDirBuf, path);
			else
				strcpy_s(workDirBuf, szFileName);

			LPSTR pstr = strrchr(workDirBuf, '\\');
			*pstr = _T('\0');
			pLink->SetWorkingDirectory(workDirBuf);
		}
		//快捷键
		if (wHotkey != 0)
			pLink->SetHotkey(wHotkey);

		//备注
		if (szDescription != nullptr)
			pLink->SetDescription(szDescription);

		//显示方式
		pLink->SetShowCmd(iShowCmd);

		//快捷方式的路径 + 名称
		char szBuffer[MAX_PATH];
		if (szLnkFileName != nullptr) //指定了快捷方式的名称
			sprintf_s(szBuffer, "%s\\%s", szLnkFileDir, szLnkFileName);
		else
		{
			//没有指定名称，就从取指定文件的文件名作为快捷方式名称。
			LPCSTR pstr;
			if (szFileName != nullptr)
				pstr = strrchr(szFileName, '\\');
			else
			{
				char path[MAX_PATH];
				GetModuleFileName(nullptr, path, MAX_PATH);
				pstr = strrchr(path, '\\');
			}

			if (pstr == nullptr)
			{
				ppf->Release();
				pLink->Release();
				return FALSE;
			}
			//注意后缀名要从.exe改为.lnk
			sprintf_s(szBuffer, "%s%s", szLnkFileDir, pstr);
			int nLen = strlen(szBuffer);
			szBuffer[nLen - 3] = 'l';
			szBuffer[nLen - 2] = 'n';
			szBuffer[nLen - 1] = 'k';
		}

		hr = ppf->Save(String::MulitToWide(szBuffer).c_str(), TRUE);

		ppf->Release();
		pLink->Release();
		return SUCCEEDED(hr);
	}

	void* Windows::GetCurrentModule(bool bRef)
	{
		HMODULE hModule = nullptr;
		if (GetModuleHandleEx(bRef ? GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS : (GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT),
			(LPCSTR)GetCurrentModule, &hModule))
			return hModule;

		return nullptr;
	}

	bool Windows::SearchFile(const char* szFolder, const char* szFilter, std::vector<_HLFILEINFO*>* fileList)
	{
		_chdir(szFolder);
		long hFile;
		_finddata_t fileinfo;
		if ((hFile = _findfirst(szFilter, &fileinfo)) != -1)
		{
			do
			{
				if (!(fileinfo.attrib & _A_SUBDIR))
				{
					_HLFILEINFO* fileInfo = new _HLFILEINFO();
					strcpy_s(fileInfo->name, fileinfo.name);

					tm access;
					_localtime64_s(&access, &fileinfo.time_access);
					strftime(fileInfo->date, 128, "%Y-%m-%d %T", &access);

					fileInfo->size = fileinfo.size;

					char filePath[MAX_PATH];
					sprintf_s(filePath, "%s%s", szFolder, fileinfo.name);

					VS_FIXEDFILEINFO *pVsInfo;
					unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
					int iVerInfoSize = GetFileVersionInfoSizeA(filePath, NULL);
					if (iVerInfoSize != 0)
					{
						char *pBuf = nullptr;
						while (!pBuf) pBuf = new char[iVerInfoSize];

						if (GetFileVersionInfoA(filePath, 0, iVerInfoSize, pBuf))
						{
							if (VerQueryValueA(pBuf, "\\", (void **)&pVsInfo, &iFileInfoSize))
							{
								sprintf_s(pBuf, MAX_PATH, "%d.%d.%d.%d", HIWORD(pVsInfo->dwFileVersionMS), LOWORD(pVsInfo->dwFileVersionMS), HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));
								strcpy_s(fileInfo->version, pBuf);
							}
						}
						delete pBuf;
					}

					fileList->push_back(fileInfo);
				}
				else if ((fileinfo.attrib & _A_SUBDIR))
				{
					if (strcmp(fileinfo.name, ".") != 0 && strcmp
					(fileinfo.name, "..") != 0)
					{
						char subdir[_MAX_PATH];
						strcpy_s(subdir, szFolder);
						strcat_s(subdir, fileinfo.name);
						strcat_s(subdir, "\\");
						if (!SearchFile(subdir, szFilter, fileList))
							return false;
					}
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}

		return true;
	}

	void Windows::CenterWindow(void* hWnd)
	{
		int scrWidth, scrHeight;
		RECT rect;
		//获得屏幕尺寸
		scrWidth = GetSystemMetrics(SM_CXSCREEN);
		scrHeight = GetSystemMetrics(SM_CYSCREEN);
		//取得窗口尺寸
		GetWindowRect((HWND)hWnd, &rect);
		//重新设置rect里的值
		rect.left = (scrWidth - rect.right) / 2;
		rect.top = (scrHeight - rect.bottom) / 2;
		//移动窗口到指定的位置
		SetWindowPos((HWND)hWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
	}

	bool Windows::isStarted(const char* szClass)
	{
		HANDLE mutex = CreateMutex(nullptr, TRUE, szClass);
		if ((mutex != nullptr) && (GetLastError() == ERROR_ALREADY_EXISTS))
		{
			::CloseHandle(mutex);
			mutex = 0;
			return true;
		}
		return false;
	}

	int Windows::AddTrayIcon(_TRAYICONINFO icon)
	{
		NOTIFYICONDATA nid;
		nid.cbSize = sizeof(nid);
		nid.hWnd = (HWND)icon.hWnd;
		nid.uID = 0;
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_SHOWTIP;
		nid.uCallbackMessage = WM_USER;
		nid.hIcon = (HICON)LoadImage(nullptr, icon.szIcon, IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		lstrcpy(nid.szTip, icon.szTitle);
		Shell_NotifyIcon(NIM_ADD, &nid);
		int id = m_TrayIcons.size();
		m_TrayIcons.insert(std::make_pair(id, icon));
		return id;
	}

	void Windows::SetTrayIcon(int nID, _TRAYICONINFO icon)
	{
		for (auto it = m_TrayIcons.begin(); it != m_TrayIcons.end();)
		{
			if (it->first == nID)
			{
				NOTIFYICONDATA nid;
				nid.cbSize = sizeof(nid);
				nid.uID = 0;
				nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_SHOWTIP;
				nid.uCallbackMessage = WM_USER;
				if (icon.hWnd)
					nid.hWnd = (HWND)icon.hWnd;
				else
					nid.hWnd = (HWND)it->second.hWnd;
				if (icon.szIcon)
					nid.hIcon = (HICON)LoadImage(nullptr, icon.szIcon, IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);
				else
					nid.hIcon = (HICON)LoadImage(nullptr, it->second.szIcon, IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);;
				if (icon.szTitle)
					lstrcpy(nid.szTip, icon.szTitle);
				else
					lstrcpy(nid.szTip, it->second.szTitle);
				Shell_NotifyIcon(NIM_MODIFY, &nid);
				break;
			}

			it++;
		}
	}

	void Windows::DelTrayIcon(int nID)
	{
		if (nID == -1)
		{
			for (auto it = m_TrayIcons.begin(); it != m_TrayIcons.end();)
			{
				NOTIFYICONDATA nid;
				nid.cbSize = sizeof(nid);
				nid.hWnd = (HWND)it->second.hWnd;
				Shell_NotifyIcon(NIM_DELETE, &nid);
				m_TrayIcons.erase(it++);
			}
		}
		else
		{
			for (auto it = m_TrayIcons.begin(); it != m_TrayIcons.end();)
			{
				if (it->first == nID)
				{
					NOTIFYICONDATA nid;
					nid.cbSize = sizeof(nid);
					nid.hWnd = (HWND)it->second.hWnd;
					Shell_NotifyIcon(NIM_DELETE, &nid);
					m_TrayIcons.erase(it++);
					break;
				}

				it++;
			}
		}
	}

	bool Windows::ShellExe(const char* szFile, const char* szPara, bool bShow, bool bWait)
	{
		std::string dir, file;
		RE2 re("(.+[$\\\\])([^\\\\]+exe)");
		RE2::Extract(String::GBKToUTF8(szFile).c_str(), re, "\\1", &dir);
		RE2::Extract(String::GBKToUTF8(szFile).c_str(), re, "\\2", &file);
		char a[MAX_PATH], b[MAX_PATH];
		strcpy_s(a, String::UTF8ToGBK(dir).c_str());
		strcpy_s(b, String::UTF8ToGBK(file).c_str());

		SHELLEXECUTEINFO sei;
		ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
		sei.cbSize = sizeof(SHELLEXECUTEINFO);
		if ((strcmp(a, "") == 0) || (strcmp(b, "") == 0))
		{
			sei.lpDirectory = 0;
			sei.lpFile = szFile;
		}
		else
		{
			sei.lpDirectory = a;
			sei.lpFile = b;
		}
		sei.lpVerb = "open";
		sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
		sei.lpParameters = szPara;
		if (bShow) sei.nShow = SW_SHOW;
		else sei.nShow = SW_HIDE;
		if (!ShellExecuteEx(&sei)) return false;

		if (bWait && (sei.hProcess != NULL))
		{
			WaitForSingleObject(sei.hProcess, INFINITE);
			TerminateProcess(sei.hProcess, 0);
			sei.hProcess = NULL;
		}

		return true;
	}

	void Windows::Kill(int pid)
	{
		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
		BOOL bOkey = TerminateProcess(hProcess, 0);
		if (bOkey)
		{
			WaitForSingleObject(hProcess, 1000);
			CloseHandle(hProcess);
			hProcess = NULL;
		}
	}

	int Windows::KillProcess(const char* szFilename, const char* szDir)
	{
		DWORD pid = 0;

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 process;
		ZeroMemory(&process, sizeof(process));
		process.dwSize = sizeof(process);

		std::vector<DWORD> pids;
		if (Process32First(snapshot, &process))
		{
			do
			{
				if (!strcmp(process.szExeFile, szFilename))
				{
					pid = process.th32ProcessID;
					pids.push_back(pid);
				}
			} while (Process32Next(snapshot, &process));
		}

		CloseHandle(snapshot);

		char szTemp[MAX_PATH];
		char szPath[MAX_PATH];
		if (szDir != 0) sprintf_s(szPath, "%s%s", szDir, szFilename);

		size_t size = pids.size();
		for (size_t i = 0; i < size; ++i)
		{
			GetProcessFullPath(pid, szTemp);

			if (szDir != 0)
			{
				if (!_stricmp(szTemp, szPath))
				{
					Kill(pids[i]);
					return 1;
				}
			}
			else
			{
				Kill(pids[i]);
				return 1;
			}
		}

		return 0;
	}

	int Windows::DosPathToNtPath(char* pszDosPath, char* pszNtPath)
	{
		TCHAR           szDriveStr[500];
		TCHAR           szDrive[3];
		TCHAR           szDevName[100];
		INT             cchDevName;
		INT             i;

		if (!pszDosPath || !pszNtPath)
			return FALSE;

		if (GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr))  //"szDriveStr包含各个分区c:\\, d:\\, e:\\, f:\\"
		{
			for (i = 0; szDriveStr[i]; i += 4)
			{
				if (!lstrcmpi(&(szDriveStr[i]), "A:\\") || !lstrcmpi(&(szDriveStr[i]), "B:\\"))
					continue;

				szDrive[0] = szDriveStr[i];
				szDrive[1] = szDriveStr[i + 1];
				szDrive[2] = '\0';
				if (!QueryDosDevice(szDrive, szDevName, 100))//查询 Dos 设备，并给szDevName赋值为：\\Device\\HarddiskVolume2
					return FALSE;

				cchDevName = lstrlen(szDevName);
				if (_tcsnicmp(pszDosPath, szDevName, cchDevName) == 0)//命中  
				{
					lstrcpy(pszNtPath, szDrive);//复制驱动器  
					lstrcat(pszNtPath, pszDosPath + cchDevName);//复制路径  

					return TRUE;
				}
			}
		}

		lstrcpy(pszNtPath, pszDosPath);

		return FALSE;
	}

	int Windows::GetProcessFullPath(unsigned long dwPID, char pszFullPath[260])
	{
		char       szImagePath[MAX_PATH];
		HANDLE     hProcess;

		if (!pszFullPath)
			return FALSE;

		pszFullPath[0] = '\0';
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 0, dwPID);
		if (!hProcess)
			return FALSE;

		if (!GetProcessImageFileName(hProcess, szImagePath, MAX_PATH))  //获取进程路径名（包含设备地址）\\Device\\HarddiskVolume2\\Program files\\**
		{
			CloseHandle(hProcess);
			return FALSE;
		}

		if (!DosPathToNtPath(szImagePath, pszFullPath))  //将路径名从包含设备地址的字符串中提取出来
		{
			CloseHandle(hProcess);
			return FALSE;
		}

		CloseHandle(hProcess);

		return TRUE;
	}

	int Windows::Random(int range)
	{
		srand((int)time(0));
		return rand() % range;
	}

	bool Windows::ExtractZip(const char* szFile, const char* szPath)
	{
		unzFile zipfile = unzOpen(szFile);
		if (zipfile == NULL) return false;

		unz_global_info global_info;
		if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
		{
			unzClose(zipfile);
			return false;
		}

		_chdir(szPath);

		char read_buffer[8192];

		for (uLong i = 0; i < global_info.number_entry; ++i)
		{
			// Get info about current file.
			unz_file_info file_info;
			char filename[MAX_PATH];
			if (unzGetCurrentFileInfo(zipfile, &file_info, filename, MAX_PATH, NULL, 0, NULL, 0) != UNZ_OK)
			{
				unzClose(zipfile);
				return false;
			}

			// Check if this entry is a directory or file.
			const size_t filename_length = strlen(filename);
			if (filename[filename_length - 1] == '/')
				_mkdir(filename);
			else
			{
				if (unzOpenCurrentFile(zipfile) != UNZ_OK)
				{
					unzClose(zipfile);
					return false;
				}

				// Open a file to write out the data.
				int nRetry = 5;
				FILE *out = 0;

				while (nRetry != 0)
				{
					fopen_s(&out, filename, "wb");
					if (out != NULL) break;
					nRetry--;
					Sleep(2000);
				}

				if (out == NULL)
				{
					unzCloseCurrentFile(zipfile);
					unzClose(zipfile);
					return false;
				}

				int error = UNZ_OK;
				do
				{
					error = unzReadCurrentFile(zipfile, read_buffer, 8192);
					if (error < 0)
					{
						unzCloseCurrentFile(zipfile);
						unzClose(zipfile);
						return false;
					}

					// Write data to file.
					if (error > 0)
					{
						fwrite(read_buffer, error, 1, out); // You should check return of fwrite...
					}
				} while (error > 0);

				fclose(out);
			}

			unzCloseCurrentFile(zipfile);

			// Go the the next entry listed in the zip file.
			if ((i + 1) < global_info.number_entry)
			{
				if (unzGoToNextFile(zipfile) != UNZ_OK)
				{
					unzClose(zipfile);
					return false;
				}
			}
		}

		unzClose(zipfile);

		return true;
	}
}
