// Permission.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <io.h>
#include <TlHelp32.h>
#include <conio.h>

std::wstring MulitToWide(const char* strMulite)
{
  int nSize = MultiByteToWideChar(CP_ACP, 0, strMulite, strlen(strMulite), 0, 0);
  if (nSize <= 0) return L"";

  auto pwszDst = new wchar_t[(int64_t)nSize + 1];
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

std::string WideToMulit(const wchar_t* strWide)
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

int CreateLinkFile(LPCWSTR szStartAppPath, LPCWSTR szAddCmdLine, LPCOLESTR szDestLnkPath, LPCWSTR szIconPath)
{
  int exit = 0;

	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		IShellLink* pShellLink;
		hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)& pShellLink);
		if (SUCCEEDED(hr))
		{
			pShellLink->SetPath(szStartAppPath);
			std::wstring strTmp = szStartAppPath;
			int nStart = strTmp.find_last_of(TEXT("/\\"));
			pShellLink->SetWorkingDirectory(strTmp.substr(0, nStart).c_str());
			pShellLink->SetArguments(szAddCmdLine);

			if (szIconPath) pShellLink->SetIconLocation(szIconPath, 0);

			IPersistFile* pPersistFile;
			hr = pShellLink->QueryInterface(IID_IPersistFile, (void**)& pPersistFile);
			if (SUCCEEDED(hr))
			{
				hr = pPersistFile->Save((szDestLnkPath), FALSE);
				if (SUCCEEDED(hr)) exit = 1;
				pPersistFile->Release();
			}
			pShellLink->Release();
		}
		CoUninitialize();
	}
	return exit;
}

int EnableAutoStartShotcut(LPCWSTR szFilePath, LPCWSTR szShotcut)
{
	wchar_t szTemp[MAX_PATH] = { 0 };
	swprintf_s(szTemp, L"C:/ProgramData/Microsoft/Windows/Start Menu/Programs/StartUp/%s", szShotcut);

  if (_waccess(szTemp, 0) != 0) {
    return CreateLinkFile(szFilePath, L"", szTemp, 0);
  }

  return 0;
}

int DisableAutoStartShotcut(LPCWSTR szFilePath, LPCWSTR szShotcut)
{
	wchar_t szTemp[MAX_PATH] = { 0 };
	swprintf_s(szTemp, L"C:/ProgramData/Microsoft/Windows/Start Menu/Programs/StartUp/%s", szShotcut);

  if (_waccess(szTemp, 0) == 0) {
    DeleteFile(szTemp);
    return 1;
  }

  return 0;
}

int EnableDesktopShotcut(LPCWSTR szFilePath, LPCWSTR szShotcut)
{
	wchar_t szUserName[MAX_PATH] = { 0 };
	DWORD dwSize = MAX_PATH;
	GetUserName(szUserName, &dwSize);

	wchar_t szTemp[MAX_PATH] = { 0 };
	swprintf_s(szTemp, L"C:/Users/%s/Desktop/%s", szUserName, szShotcut);

  if (_waccess(szTemp, 0) != 0) {
    return CreateLinkFile(szFilePath, L"", szTemp, 0);
  }

  return 0;
}

int DisableDesktopShotcut(LPCWSTR szFilePath, LPCWSTR szShotcut)
{
	wchar_t szUserName[MAX_PATH] = { 0 };
	DWORD dwSize = MAX_PATH;
	GetUserName(szUserName, &dwSize);

	wchar_t szTemp[MAX_PATH] = { 0 };
	swprintf_s(szTemp, L"C:/Users/%s/Desktop/%s", szUserName, szShotcut);

  if (_waccess(szTemp, 0) == 0) {
    DeleteFile(szTemp);
    return 1;
  };

  return 0;
}

int EnableAutoStartRegistry(LPCWSTR szFilePath, LPCWSTR szName)
{
  int exit = 0;

	wchar_t szReg[MAX_PATH] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	HKEY hKey;
	if (SUCCEEDED(RegOpenKeyEx(HKEY_LOCAL_MACHINE, szReg, 0, KEY_WOW64_64KEY | KEY_ALL_ACCESS, &hKey)))
	{
		DWORD dwValue;
		DWORD dwSize = sizeof(DWORD);
		DWORD dwType = REG_SZ;

		if (SUCCEEDED(RegQueryValueEx(hKey, szName, 0, &dwType, (LPBYTE)&dwValue, &dwSize)))
		{
      if (dwSize == sizeof(DWORD)) {
        RegSetValueEx(hKey, szName, 0, REG_SZ, (LPBYTE)szFilePath, wcslen(szFilePath) * sizeof(wchar_t) + 1);
        exit = 1;
      }
		}
	}
	else if(SUCCEEDED(RegOpenKeyEx(HKEY_LOCAL_MACHINE, szReg, 0, KEY_ALL_ACCESS, &hKey)))
	{
		DWORD dwValue = 0;
		DWORD dwSize = sizeof(DWORD);
		DWORD dwType = REG_SZ;
		
		if (SUCCEEDED(RegQueryValueEx(hKey, szName, 0, &dwType, (LPBYTE)&dwValue, &dwSize)))
		{
      if (dwSize == sizeof(DWORD)) {
        RegSetValueEx(hKey, szName, 0, REG_SZ, (LPBYTE)szFilePath, wcslen(szFilePath) * sizeof(wchar_t) + 1);
        exit = 1;
      }
		}
	}

	RegCloseKey(hKey);

  return exit;
}

int DisableAutoStartRegistry(LPCWSTR szFilePath, LPCWSTR szName)
{
  int exit = 0;

	wchar_t szReg[MAX_PATH] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	HKEY hKey;
	if (SUCCEEDED(RegOpenKeyEx(HKEY_LOCAL_MACHINE, szReg, 0, KEY_WOW64_64KEY | KEY_ALL_ACCESS, &hKey)))
	{
		DWORD dwValue;
		DWORD dwSize = sizeof(DWORD);
		DWORD dwType = REG_SZ;

		if (SUCCEEDED(RegQueryValueEx(hKey, szName, 0, &dwType, (LPBYTE)& dwValue, &dwSize)))
		{
      if (dwSize != sizeof(DWORD)) {
        RegDeleteValue(hKey, szName);
        exit = 1;
      }
		}
	}
	else if (SUCCEEDED(RegOpenKeyEx(HKEY_LOCAL_MACHINE, szReg, 0, KEY_ALL_ACCESS, &hKey)))
	{
		DWORD dwValue = 0;
		DWORD dwSize = sizeof(DWORD);
		DWORD dwType = REG_SZ;

		if (SUCCEEDED(RegQueryValueEx(hKey, szName, 0, &dwType, (LPBYTE)& dwValue, &dwSize)))
		{
      if (dwSize != sizeof(DWORD)) {
        RegDeleteValue(hKey, szName);
        exit = 1;
      }
		}
	}

	RegCloseKey(hKey);

  return exit;
}

void DelFile(LPCWSTR szFilePath)
{
	std::wstring strDir = szFilePath;
	if (strDir.at(strDir.length() - 1) != '\\')
		strDir += '\\';
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile((strDir + L"*.*").c_str(), &wfd);
	if (hFind == INVALID_HANDLE_VALUE) return;

	do
	{
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (_wcsicmp(wfd.cFileName, L".") != 0 &&
				_wcsicmp(wfd.cFileName, L"..") != 0)
        DeleteFile((strDir + wfd.cFileName).c_str());
		}
		else
		{
			DeleteFile((strDir + wfd.cFileName).c_str());
		}
	} while (FindNextFile(hFind, &wfd));
	FindClose(hFind);
	RemoveDirectory(szFilePath);
}

int RunGame(LPCSTR dir, LPCSTR ip, LPCSTR username, LPCSTR password)
{
  int exit = 0;

  char szPara[MAX_PATH];
  sprintf_s(szPara, "%s,%s", username, password);

  std::string s = dir;
  size_t pos = s.find_last_of("\\/");
  ShellExecuteA(0, "open", dir, szPara, s.substr(0, pos).c_str(), SW_SHOW);

  HANDLE hShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hShot != INVALID_HANDLE_VALUE)
  {
    PROCESSENTRY32 process;
    process.dwSize = sizeof(process);

    BOOL ret = Process32First(hShot, &process);
    while (ret)
    {
      if (wcscmp(process.szExeFile, L"BlackDesert64.exe") == 0)
      {
        CloseHandle(hShot);

        Sleep(1500);

        hShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process.th32ProcessID);
        if (hShot != INVALID_HANDLE_VALUE)
        {
          MODULEENTRY32 module;
          module.dwSize = sizeof(module);

          ret = Module32First(hShot, &module);
          while (ret)
          {
            if (wcscmp(module.szModule, L"BlackDesert64.exe") == 0)
            {
              exit = 1;

              CloseHandle(hShot);

              HANDLE hProcess = OpenProcess(0x1F0FFF, FALSE, module.th32ProcessID);
              if (hProcess)
              {
                DWORD64 dwBaseAddr = (DWORD64)module.modBaseAddr;
                SIZE_T size = 0;
                DWORD64 dwRead;

                // Crypto
                if (ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddr + 10654470L), &dwRead, sizeof(DWORD64), &size))
                {
                  BYTE byte[] = { 0x90, 0x90 };
                  if (!WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddr + 10654470L), byte, sizeof(byte) * sizeof(BYTE), &size))
                  {
                    exit = 0;
                    CloseHandle(hProcess);
                    break;
                  }
                }

                // XC
                if (ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddr + 8018698L), &dwRead, sizeof(DWORD64), &size))
                {
                  BYTE byte[] = { 0x90, 0x90 };
                  if (!WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddr + 8018698L), byte, sizeof(byte) * sizeof(BYTE), &size))
                  {
                    exit = 0;
                    CloseHandle(hProcess);
                    break;
                  }
                }

                // XC
                if (ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddr + 8018928L), &dwRead, sizeof(DWORD64), &size))
                {
                  BYTE byte[] = { 0xEB };
                  if (!WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddr + 8018928L), byte, sizeof(byte) * sizeof(BYTE), &size))
                  {
                    exit = 0;
                    CloseHandle(hProcess);
                    break;
                  }
                }

                // Wipe IP
                if (ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddr + 45357624L), &dwRead, sizeof(DWORD64), &size))
                {
                  BYTE byte[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
                  if (!WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddr + 45357624L), byte, sizeof(byte) * sizeof(BYTE), &size))
                  {
                    exit = 0;
                    CloseHandle(hProcess);
                    break;
                  }
                }

                // Server IP
                char s[MAX_PATH] = { 0 };
                if (ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddr + 45357624L), &s, MAX_PATH, &size))
                {
                  std::string str = ip;
                  BYTE* byte = (BYTE*)str.c_str();
                  if (!WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddr + 45357624L), byte, str.length(), &size))
                  {
                    exit = 0;
                    CloseHandle(hProcess);
                    break;
                  }
                }

                CloseHandle(hProcess);
              }

              break;
            }

            ret = Module32Next(hShot, &module);
          }
        }

        break;
      }

      ret = Process32Next(hShot, &process);
    }
  }

  return exit;
}

int RegisterProtocol()
{
  int exit = 0;

  wchar_t szReg[MAX_PATH] = L"BDOLauncher";
  DWORD dwDisposition;
  HKEY hKey = 0;
  if (RegCreateKey(HKEY_CLASSES_ROOT, szReg, &hKey) == ERROR_SUCCESS)
  {
    wchar_t szPath[MAX_PATH];
    GetModuleFileName(NULL, szPath, MAX_PATH);

    if (RegSetKeyValue(hKey, 0, L"URL Protocol", REG_SZ, L"", 0) == ERROR_SUCCESS)
    {
      if (RegSetValue(hKey, 0, REG_SZ, L"URL:BOO Launcher Protocol", wcslen(L"URL:BOO Launcher Protocol") * sizeof(wchar_t)) == ERROR_SUCCESS)
      {
        RegCloseKey(hKey);

        wchar_t szSubreg[MAX_PATH];
        swprintf_s(szSubreg, L"%s\\DefaultIcon", szReg);
        if (RegCreateKey(HKEY_CLASSES_ROOT, szSubreg, &hKey) == ERROR_SUCCESS)
        {
          wchar_t szVal[MAX_PATH];
          swprintf_s(szVal, L"\"%s\",0", szPath);
          if (RegSetValue(hKey, 0, REG_SZ, szVal, MAX_PATH) == ERROR_SUCCESS)
          {
            RegCloseKey(hKey);

            swprintf_s(szSubreg, L"%s\\shell\\open\\command", szReg);
            if (RegCreateKey(HKEY_CLASSES_ROOT, szSubreg, &hKey) == ERROR_SUCCESS)
            {
              swprintf_s(szVal, L"\"%s\" \"%%1\"", szPath);
              if (RegSetValue(hKey, 0, REG_SZ, szVal, MAX_PATH) == ERROR_SUCCESS)
              {
                exit = 1;
              }
            }
          }
        }
      }
    }
  }

  RegCloseKey(hKey);

  return exit;
}

void replace(std::string& strBase, std::string strSrc, std::string strDes)
{
  std::string::size_type pos = 0;
  std::string::size_type srcLen = strSrc.size();
  std::string::size_type desLen = strDes.size();
  pos = strBase.find(strSrc, pos);
  while ((pos != std::string::npos))
  {
    strBase.replace(pos, srcLen, strDes);
    pos = strBase.find(strSrc, (pos + desLen));
  }
}

int checkVersion(std::string remote, std::string local)
{
  int exit = 0;


  return exit;
}

int main(int argc, LPCSTR argv[])
{
  // BDOLauncher://E:/BDO_v795/bin64/BlackDesert64.exe&127.0.0.1&Username&Password

  if (argc < 2) {
    RegisterProtocol();
    ShellExecute(NULL, L"open", L"explorer.exe", L"http://127.0.0.1:3000", NULL, SW_SHOW);
    return 0;
  }

  std::string str = argv[1];

	if (strcmp(argv[1], "EnableAutoStartShotcut") == 0)
		return EnableAutoStartShotcut(MulitToWide(argv[2]).c_str(), MulitToWide(argv[3]).c_str());
	else if (strcmp(argv[1], "DisableAutoStartShotcut") == 0)
    return DisableAutoStartShotcut(MulitToWide(argv[2]).c_str(), MulitToWide(argv[3]).c_str());
	else if (strcmp(argv[1], "EnableDesktopShotcut") == 0)
    return EnableDesktopShotcut(MulitToWide(argv[2]).c_str(), MulitToWide(argv[3]).c_str());
	else if (strcmp(argv[1], "DisableDesktopShotcut") == 0)
    return DisableDesktopShotcut(MulitToWide(argv[2]).c_str(), MulitToWide(argv[3]).c_str());
	else if (strcmp(argv[1], "EnableAutoStartRegistry") == 0)
    return EnableAutoStartRegistry(MulitToWide(argv[2]).c_str(), MulitToWide(argv[3]).c_str());
	else if (strcmp(argv[1], "DisableAutoStartRegistry") == 0)
    return DisableAutoStartRegistry(MulitToWide(argv[2]).c_str(), MulitToWide(argv[3]).c_str());
	else if (strcmp(argv[1], "DeleteFile") == 0)
    DelFile(MulitToWide(argv[2]).c_str());
  else if (strcmp(argv[1], "RunGame") == 0)
    return RunGame(argv[2], argv[3], argv[4], argv[5]);
  else if (strcmp(argv[1], "RegisterProtocol") == 0)
    return RegisterProtocol();
  else if (str.find("bdolauncher://") != std::string::npos)
  {
    std::string str = argv[1];
    std::string ip, path, username, password;

    replace(str, "%20", " ");

    char sPath[MAX_PATH];
    GetModuleFileNameA(NULL, sPath, MAX_PATH);
    path = sPath;
    size_t nStart1 = path.find_last_of('\\');
    path = path.substr(0, nStart1);
    sprintf_s(sPath, "%s\\bin64\\BlackDesert64.exe", path.c_str());

    size_t nStart = str.find('/') + 2;
    size_t nEnd = str.find('&');
    size_t offset = str.find('/', nStart);
    ip = str.substr(nStart, nEnd - nStart);

    nStart = nEnd + 1;
    nEnd = str.find('&', nStart);
    username = str.substr(nStart, nEnd - nStart);

    nStart = nEnd + 1;
    password = str.substr(nStart, str.length() - nStart - 1);

#ifdef _DEBUG
    printf_s("%s %s %s %s", sPath, ip.c_str(), username.c_str(), password.c_str());
#else
    RunGame(sPath, ip.c_str(), username.c_str(), password.c_str());
#endif
  }

#ifdef _DEBUG
  _getch();
#endif

  return 0;
}
