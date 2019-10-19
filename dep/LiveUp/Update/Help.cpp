#include "Help.h"

#include <windows.h>
#include <io.h>
#include <unzip.h>
#include <direct.h>
#include <shellapi.h>
#include <time.h>
#include <TlHelp32.h>
#include <Psapi.h>

Help& Help::GetInstance()
{
	static Help instance;
	return instance;
}

Help::Help()
{
	m_pFile = 0;
}

Help::~Help()
{
	fclose(m_pFile);
	m_pFile = 0;
}

bool Help::isStarted(const wchar_t* szClass)
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

void Help::Kill(int pid)
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

bool Help::KillProcess(const wchar_t* szFilename, const wchar_t* szDir)
{
	DWORD pid = 0;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);

	std::vector<DWORD> pids;
	if (Process32FirstW(snapshot, &process))
	{
		do
		{
			if (!wcscmp(process.szExeFile, szFilename))
			{
				pid = process.th32ProcessID;
				pids.push_back(pid);
			}
		} while (Process32NextW(snapshot, &process));
	}

	CloseHandle(snapshot);

	wchar_t szTemp[MAX_PATH];
	wchar_t szPath[MAX_PATH];
	if (szDir != 0) swprintf_s(szPath, L"%s\%s", szDir, szFilename);

	size_t size = pids.size();
	for (size_t i = 0; i < size; ++i)
	{
		GetProcessFullPath(pid, szTemp);

		Log("%s - %s", WideToMulit(szPath).c_str(), WideToMulit(szTemp).c_str());
		if (szDir != 0)
		{
			if (!_wcsicmp(szTemp, szPath))
			{
				Kill(pids[i]);
				return true;
			}
		}
		else
		{
			Kill(pids[i]);
			return true;
		}
	}

	return false;
}

int Help::DosPathToNtPath(wchar_t* pszDosPath, wchar_t* pszNtPath)
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
			if (!lstrcmpi(&(szDriveStr[i]), L"A:\\") || !lstrcmpi(&(szDriveStr[i]), L"B:\\"))
				continue;

			szDrive[0] = szDriveStr[i];
			szDrive[1] = szDriveStr[i + 1];
			szDrive[2] = '\0';
			if (!QueryDosDevice(szDrive, szDevName, 100))//查询 Dos 设备，并给szDevName赋值为：\\Device\\HarddiskVolume2
				return FALSE;

			cchDevName = lstrlen(szDevName);
			if (_wcsnicmp(pszDosPath, szDevName, cchDevName) == 0)//命中  
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

int Help::GetProcessFullPath(unsigned long dwPID, wchar_t pszFullPath[260])
{
	TCHAR       szImagePath[MAX_PATH];
	HANDLE      hProcess;

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

bool Help::GetAppPath(wchar_t* szPath)
{
	GetModuleFileNameW(nullptr, szPath, MAX_PATH);
	(wcsrchr(szPath, L'\\'))[1] = 0;
	if (szPath != L"") return true;
	return false;
}

std::string Help::UTF8ToGBK(const std::string& strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, nullptr, 0);
	auto str1 = new wchar_t[len + 1];
	memset(str1, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, str1, len);

	len = WideCharToMultiByte(CP_ACP, 0, str1, -1, nullptr, 0, nullptr, nullptr);
	auto str2 = new char[len + 1];
	memset(str2, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, str1, -1, str2, len, nullptr, nullptr);

	std::string strTemp(str2);
	delete[] str2;
	str2 = nullptr;
	delete[] str1;
	str1 = nullptr;
	return strTemp;
}

std::string Help::GBKToUTF8(const std::string& strGBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, nullptr, 0);
	auto str1 = new wchar_t[len + 1];
	memset(str1, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, len);

	len = WideCharToMultiByte(CP_UTF8, 0, str1, -1, nullptr, 0, nullptr, nullptr);
	auto str2 = new char[len + 1];
	memset(str2, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, len, nullptr, nullptr);

	std::string strTemp(str2);
	delete[] str1;
	str1 = nullptr;
	delete[] str2;
	str2 = nullptr;
	return strTemp;
}

std::string Help::WideToMulit(const wchar_t* strWide)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, strWide, -1, nullptr, 0, nullptr, nullptr);

	if (nLen <= 0) return std::string("");

	auto pszDst = new char[nLen];
	if (pszDst == nullptr) return std::string("");

	WideCharToMultiByte(CP_ACP, 0, strWide, -1, pszDst, nLen, nullptr, nullptr);
	pszDst[nLen - 1] = 0;

	std::string strTemp(pszDst);
	delete[] pszDst;
	pszDst = nullptr;
	return strTemp;
}

std::wstring Help::MulitToWide(const char* strMulite)
{
	int nSize = MultiByteToWideChar(CP_ACP, 0, strMulite, strlen(strMulite), 0, 0);
	if (nSize <= 0) return L"";

	auto pwszDst = new wchar_t[nSize + 1];
	if (pwszDst == nullptr) return L"";

	MultiByteToWideChar(CP_ACP, 0, strMulite, strlen(strMulite), pwszDst, nSize);
	pwszDst[nSize] = 0;

	if (pwszDst[0] == 0xFEFF) // skip Oxfeff  
		for (int i = 0; i < nSize; i++)
			pwszDst[i] = pwszDst[i + 1];

	std::wstring wcharString(pwszDst);
	delete[] pwszDst;
	pwszDst = nullptr;
	return wcharString;
}

bool Help::ShellExe(const wchar_t* szFile)
{
	SHELLEXECUTEINFO sei;
	ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.lpDirectory = L"";
	sei.lpFile = szFile;
	sei.lpVerb = L"open";
	sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
	sei.lpParameters = L"";
	sei.nShow = SW_SHOW;
	if (!ShellExecuteEx(&sei)) return false;

	return true;
}

bool Help::ExtractZip(const char* szFile, const char* szPath)
{
	unzFile zipfile = unzOpen64(szFile);
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
			Log("获取文件%s信息失败", filename);
			unzClose(zipfile);
			return false;
		}

		sprintf_s(filename, "%s", Help::UTF8ToGBK(filename).c_str());

		// Check if this entry is a directory or file.
		const size_t filename_length = strlen(filename);
		if (filename[filename_length - 1] == '/')
			_mkdir(filename);
		else
		{
			if (unzOpenCurrentFile(zipfile) != UNZ_OK)
			{
				Log("打开文件失败");
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
				Log("打开文件失败");
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
					Log("读取当前文件失败");
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
				Log("打开下一个文件失败");
				unzClose(zipfile);
				return false;
			}
		}
	}

	unzClose(zipfile);

	return true;
}

void Help::Log(const char* sFormat, ...)
{
	va_list args;
	va_start(args, sFormat);

	int len = _vscprintf(sFormat, args) + 1;
	auto str = (char*)malloc(len * sizeof(char));
	vsprintf_s(str, len, sFormat, args);

	va_end(args);

	if (m_pFile == 0)
	{
		fopen_s(&m_pFile, "./log.log", "w");
		if (m_pFile == 0) return;
		char header[3] = { (char)0xEF, (char)0xBB, (char)0xBF };
		fwrite(header, sizeof(char), 3, m_pFile);
	}

	tm tmt;
	time_t now;
	time(&now);
	_localtime64_s(&tmt, &now);

	char s1[2048];
	char s2[2048];
	sprintf_s(s1, "%d-%02d-%02d %02d:%02d:%02d %s", tmt.tm_year + 1900, tmt.tm_mon + 1, tmt.tm_mday, tmt.tm_hour, tmt.tm_min, tmt.tm_sec, str);
	sprintf_s(s2, "%s", GBKToUTF8(s1).c_str());

#ifdef _DEBUG
	fwrite(s2, sizeof(char) * strlen(s2), 1, m_pFile);
	fwrite("\r\n", 1, 2, m_pFile);
	fflush(m_pFile);
	OutputDebugStringA("LOG: ");
	OutputDebugStringA(s1);
	OutputDebugStringA("\n");
#else
	fwrite(s2, sizeof(char) * strlen(s2), 1, m_pFile);
	fwrite("\r\n", 1, 2, m_pFile);
	fflush(m_pFile);
#endif // DEBUG

	free(str);
}

bool Help::SearchFile(const char* szFolder, const char* szFilter, std::vector<_HLFILEINFO*> fileList)
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
					delete[] pBuf;
				}

				fileList.push_back(fileInfo);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}

	_chdir(szFolder);
	if ((hFile = _findfirst(szFilter, &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
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
