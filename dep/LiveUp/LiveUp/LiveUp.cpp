// Live.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "resource.h"
#include "LiveUp.h"
#include "CDialogWnd.h"

#include <string>

#include <LogManager.h>
#include <HLInternet.h>
#include <HLString.h>
#include <HLWindows.h>
#include <HLINIReader.h>
#include <JSONManager.h>
#include <re2.h>
#include <shellapi.h>
#include <direct.h>
#include <vcruntime_exception.h>
#include <future>
#include <tinyxml.h>
#include <WinUser.h>

int UpdateProgress = 0;
HINSTANCE hInst;

LiveUp &LiveUp::GetInstance()
{
	static LiveUp instance;
	return instance;
}

LiveUp::LiveUp()
{
	LogManager::GetInstance().Init("LiveUp");
	LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, "初始化更新管理器");
}

LiveUp::~LiveUp()
{
	LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, "关闭更新管理器");
}

int LiveUp::Init(const char* url1, const char* url2)
{
	if (HL::Internet::GetInstance().Get(url1) != 0)
	{
		if (HL::Internet::GetInstance().Get(url2) != 0)
		{
			LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, "无法检查更新");
			return 2;
		}
	}

	int nID = JSONManager::GetInstance().Create(HL::Internet::GetInstance().GetResponse().c_str());
	if (nID == -1)
	{
		LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, "获取更新列表失败");
		return 2;
	}

	INIReader reader("launcher.ini");
	std::string sTemp = HL::String::UTF8ToGBK(reader.Get("Config", "Launcher", "").c_str());

	wchar_t szPath[MAX_PATH];
	HL::Windows::GetAppPath(szPath);
	std::vector<HL::Windows::_HLFILEINFO*> fileInfo;

	std::string exe, dir;
	RE2 re1("(.*[$/])([^/]+exe)");
	RE2::Extract(HL::String::GBKToUTF8(sTemp.c_str()), re1, "\\2", &exe);
	RE2::Extract(HL::String::GBKToUTF8(sTemp.c_str()), re1, "\\1", &dir);
	swprintf_s(szPath, L"%s%s", szPath, HL::String::MulitToWide(dir.c_str()).c_str());
	if (!HL::Windows::SearchFile(HL::String::WideToMulit(szPath).c_str(), HL::String::UTF8ToGBK(exe.c_str()).c_str(), &fileInfo))
	{
		LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, "查找文件失败");
		return 2;
	}

	auto root = JSONManager::GetInstance().GetDocment(nID)->GetObjectW();
	if (JSONManager::GetInstance().has(nID, fileInfo[0]->version))
	{
		auto obj = root[fileInfo[0]->version].GetObjectW();
		isMandatory = obj["mandatory"].GetBool();
		swprintf_s(m_szInfo, L"%s", HL::String::MulitToWide(obj["changelog"].GetString()).c_str());
		swprintf_s(m_szOldVersion, L"%s", HL::String::MulitToWide(fileInfo[0]->version).c_str());
		swprintf_s(m_szNewVersion, L"%s", HL::String::MulitToWide(obj["version"].GetString()).c_str());

		rapidjson::StringBuffer sbBuf;
		rapidjson::Writer<rapidjson::StringBuffer> jWriter(sbBuf);
		root[fileInfo[0]->version].Accept(jWriter);

		return Check(CompareVersion(fileInfo[0]->version, obj["version"].GetString()), sbBuf.GetString());
	}
	else
	{
		auto obj = root["latest"].GetObjectW();
		isMandatory = obj["mandatory"].GetBool();
		swprintf_s(m_szInfo, L"%s", HL::String::MulitToWide(obj["changelog"].GetString()).c_str());
		swprintf_s(m_szOldVersion, L"%s", HL::String::MulitToWide(fileInfo[0]->version).c_str());
		swprintf_s(m_szNewVersion, L"%s", HL::String::MulitToWide(obj["version"].GetString()).c_str());

		rapidjson::StringBuffer sbBuf;
		rapidjson::Writer<rapidjson::StringBuffer> jWriter(sbBuf);
		root["latest"].Accept(jWriter);

		return Check(CompareVersion(fileInfo[0]->version, obj["version"].GetString()), sbBuf.GetString());
	}

	return 2;
}

int LiveUp::CompareVersion(const char* sVer1, const char* sVer2)
{
	_VERINFO ver1, ver2;
	std::string tmp;

	RE2 re("(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)");

	RE2::Extract(sVer1, re, "\\1", &tmp);
	ver1.Major = atoi(tmp.c_str());
	RE2::Extract(sVer1, re, "\\2", &tmp);
	ver1.Minor = atoi(tmp.c_str());
	RE2::Extract(sVer1, re, "\\3", &tmp);
	ver1.Revision = atoi(tmp.c_str());
	RE2::Extract(sVer1, re, "\\4", &tmp);
	ver1.Build = atoi(tmp.c_str());

	RE2::Extract(sVer2, re, "\\1", &tmp);
	ver2.Major = atoi(tmp.c_str());
	RE2::Extract(sVer2, re, "\\2", &tmp);
	ver2.Minor = atoi(tmp.c_str());
	RE2::Extract(sVer2, re, "\\3", &tmp);
	ver2.Revision = atoi(tmp.c_str());
	RE2::Extract(sVer2, re, "\\4", &tmp);
	ver2.Build = atoi(tmp.c_str());

	if (ver1.Major > ver2.Major) return 1;
	else if (ver1.Major < ver2.Major) return 2;
	if (ver1.Major > ver2.Major) return 1;
	else if (ver1.Minor < ver2.Minor) return 2;
	if (ver1.Minor > ver2.Minor) return 1;
	else if (ver1.Revision < ver2.Revision) return 2;
	if (ver1.Build > ver2.Build) return 1;
	else if (ver1.Build < ver2.Build) return 2;

	return 0;
}

bool LiveUp::ExtractResource()
{
	HRSRC hRes = FindResource((HMODULE)HL::Windows::GetCurrentModule(), MAKEINTRESOURCE(IDR_ZIPRES1), TEXT("EXE"));
	if (!hRes)
	{
		LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, "无法找到资源");
		return false;
	}

	DWORD dwSize = SizeofResource((HMODULE)HL::Windows::GetCurrentModule(), hRes);
	if (dwSize == 0) 
	{
		LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, "资源大小为0");
		return false;
	}

	HGLOBAL hGlobal = LoadResource((HMODULE)HL::Windows::GetCurrentModule(), hRes);
	if (!hGlobal) 
	{
		LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, "无法载入资源");
		return false;
	}

	LPVOID lp = LockResource(hGlobal);
	if (!lp) 
	{
		LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, "无法锁定资源");
		return false;
	}

	wchar_t szPath[MAX_PATH];
	HL::Windows::GetAppPath(szPath);
	swprintf_s(szPath, L"%sUpdate", szPath);
	if (_access(HL::String::WideToMulit(szPath).c_str(), 0) != 0)
		_mkdir(HL::String::WideToMulit(szPath).c_str());
	swprintf_s(szPath, L"%s\\Update.exe", szPath);

	FILE* fp = 0;
	fopen_s(&fp, HL::String::WideToMulit(szPath).c_str(), "wb");
	if (!fp) 
	{
		LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, "无法写入文件");
		return false;
	}
	fwrite(lp, dwSize, 1, fp);
	fclose(fp);

	FreeResource(hGlobal);

	return true;
}

void LiveUp::ShowDlg()
{
	CPaintManagerUI::SetInstance(hInst);

	HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), MAKEINTRESOURCE(IDR_ZIPRES1), _T("ZIPRES"));
	if (hResource == NULL)
		return;
	DWORD dwSize = 0;
	HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
	if (hGlobal == NULL)
	{
		::FreeResource(hResource);
		return;
	}
	dwSize = ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource);
	if (dwSize == 0)
		return;
	LPBYTE g_lpResourceZIPBuffer = new BYTE[dwSize];
	if (g_lpResourceZIPBuffer != NULL)
	{
		::CopyMemory(g_lpResourceZIPBuffer, (LPBYTE)::LockResource(hGlobal), dwSize);
	}
	::FreeResource(hResource);
	CPaintManagerUI::SetResourceZip(g_lpResourceZIPBuffer, dwSize);

	CWndShadow::Initialize(hInst);

	CDialogWnd duiFrame(m_szInfo, m_szOldVersion, m_szNewVersion);
	m_DlgHWND = duiFrame.Create(NULL, L"Liveup", UI_WNDSTYLE_FRAME, UI_WNDSTYLE_EX_FRAME);

	DWORD dwStyle = GetWindowLong(duiFrame.GetHWND(), GWL_STYLE);//获取旧样式
	WORD dwNewStyle = WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	dwNewStyle &= dwStyle;//按位与将旧样式去掉
	SetWindowLong(duiFrame.GetHWND(), GWL_STYLE, dwNewStyle);//设置成新的样式
	DWORD dwExStyle = GetWindowLong(duiFrame.GetHWND(), GWL_EXSTYLE);//获取旧扩展样式
	DWORD dwNewExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
	dwNewExStyle &= dwExStyle;//按位与将旧扩展样式去掉
	SetWindowLong(duiFrame.GetHWND(), GWL_EXSTYLE, dwNewExStyle);//设置新的扩展样式

	duiFrame.CenterWindow();
	duiFrame.ShowWindow();

	CPaintManagerUI::MessageLoop();
}

int CheckUpdate(const char* url1, const char* url2)
{
	return LiveUp::GetInstance().Init(url1, url2);
}

void DownloadProgress(int nPos)
{
	UpdateProgress = nPos;
	SendMessage(LiveUp::GetInstance().m_DlgHWND, 0x099, nPos, 0);
}

int LiveUp::Check(int nResult, const char* szJson)
{
	switch (nResult)
	{
	case 1:
		LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, "本地版本大于网络版本");
		break;
	case 2:
		LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, "本地版本小于网络版本");

		switch (MessageBox(nullptr, L"发现新版本，是否立即更新？", L"Liveup", MB_ICONQUESTION | MB_OKCANCEL))
		{
		case IDOK:
		{
			HL::Internet::GetInstance().DownloadProgress = DownloadProgress;

			std::thread t([=]()->int
			{
				if (!szJson) return 2;

				wchar_t szPath[MAX_PATH];
				HL::Windows::GetAppPath(szPath);

				int nID = JSONManager::GetInstance().Create(szJson);
				if (nID == -1)
				{
					LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, "获取更新列表失败");
					return 2;
				}

				char patchPath[MAX_PATH], updatePath[MAX_PATH], strCommand[MAX_PATH];

				sprintf_s(updatePath, "%sUpdate", HL::String::WideToMulit(szPath).c_str());
				if (_access(updatePath, 0) != 0)
					_mkdir(updatePath);

				std::string exe;
				RE2 re1("([^/]+exe)");
				RE2::Extract(HL::String::GBKToUTF8(JSONManager::GetInstance().GetString(nID, "url")), re1, "\\1", &exe);
				if (exe.length() > 0)
				{
					sprintf_s(patchPath, "%s\\%s", updatePath, HL::String::UTF8ToGBK(exe).c_str());
					if (HL::Internet::GetInstance().DownloadFile(JSONManager::GetInstance().GetString(nID, "url"), patchPath))
					{
						switch (MessageBox(nullptr, L"下载完毕是否立即更新？", L"LiveUp", MB_ICONQUESTION | MB_OKCANCEL))
						{
						case IDOK:
						{
							HL::Windows::ShellExe(patchPath, "", false);
							LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, patchPath);
							return 1;
						}
						case IDCANCEL:
							LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, "取消更新");
							return 2;
						};
					}
					return 2;
				};

				sprintf_s(updatePath, "%sUpdate\\Update.exe", HL::String::WideToMulit(szPath).c_str());
				if (!HL::Internet::GetInstance().DownloadFile(JSONManager::GetInstance().GetString(nID, "updater"), updatePath))
				{
					LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, "获取更新程序失败");
					return 2;
				}

				if (JSONManager::GetInstance().has(nID, "filelist"))
				{
					if (HL::Internet::GetInstance().DownloadFile(JSONManager::GetInstance().GetString(nID, "filelist"), "./Update/filelist.xml"))
					{
						TiXmlDocument *myDocument = new TiXmlDocument("filelist.xml");
						myDocument->LoadFile();
						TiXmlNode* node = myDocument->FirstChild("FileList");
						for (node = node->FirstChild(); node != 0; node = node->NextSibling())
						{
							TiXmlElement* element = node->ToElement();
						}

						char strCommand[MAX_PATH];
						sprintf_s(updatePath, "%sUpdate\\Update.exe", HL::String::WideToMulit(szPath).c_str());
						sprintf_s(strCommand, "Patch %s %s", HL::String::WideToMulit(szPath).c_str(), HL::String::WideToMulit(szPath).c_str());
						HL::Windows::ShellExe(updatePath, strCommand, false);
						return 1;
					}
					return 2;
				}
				else
				{
					std::string zip;
					RE2 re1("([^/]+zip)");
					RE2::Extract(HL::String::GBKToUTF8(JSONManager::GetInstance().GetString(nID, "url")), re1, "\\1", &zip);

					sprintf_s(patchPath, "%sUpdate", HL::String::WideToMulit(szPath).c_str());
					if (_access(patchPath, 0) != 0)
						_mkdir(patchPath);

					sprintf_s(patchPath, "%s\\%s", patchPath, HL::String::UTF8ToGBK(zip).c_str());
					if (HL::Internet::GetInstance().DownloadFile(JSONManager::GetInstance().GetString(nID, "url"), patchPath))
					{
						switch (MessageBox(nullptr, L"下载完毕是否立即更新？", L"LiveUp", MB_ICONQUESTION | MB_OKCANCEL))
						{
						case IDOK:
						{
							sprintf_s(strCommand, "Update %s %s %s", patchPath, HL::String::WideToMulit(szPath).c_str(), JSONManager::GetInstance().GetString(nID, "launcher"));
							HL::Windows::ShellExe(updatePath, strCommand, false);
							LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, "%s %s", updatePath, strCommand);
							return 1;
						}
						case IDCANCEL:
							LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, "取消更新");
							return 2;
						}
					}
					return 2;
				}

				return 2;
			});
			t.detach();

			LiveUp::GetInstance().ShowDlg();
			break;
		}
		case IDCANCEL:
			if (LiveUp::GetInstance().isMandatory)
			{
				MessageBox(nullptr, L"需要进行更新才能启动", L"Liveup", MB_ICONEXCLAMATION | MB_OK);
				return 2;
			}
			break;
		}
	default:
		LogManager::GetInstance().Log("LiveUp", LogManager::LOGINFO, "本地版本等于网络版本");
		break;
	}
	return 0;
}
