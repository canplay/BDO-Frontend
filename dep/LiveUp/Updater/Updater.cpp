// Update.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "resource.h"
#include "help.h"
#include "diff.h"
#include "hpatch.h"
#include "file_for_patch.h"
#include "compress_plugin.h"
#include "decompress_plugin.h"
#include "Md5.h"

#include <JsonManager.h>
#include <LogManager.h>
#include <shellapi.h>
#include <future>
#include <direct.h>
#include <corecrt_io.h>

#define MAX_LOADSTRING 100
#define _free_mem(p) { if (p) { free(p); p=0; } }
#define kPatchCacheSize_min      (hpatch_kStreamCacheSize*8)
#define kPatchCacheSize_bestmin  ((size_t)1<<21)
#define kPatchCacheSize_default  ((size_t)1<<27)
#define kPatchCacheSize_bestmax  ((size_t)1<<30)

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING] = L"自动更新";                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING] = L"HLLiveUp";            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

bool SearchFile(const char* szFolder, const char* szFilter, std::vector<std::string>* fileList)
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
				std::string temp = fileinfo.name;
				fileList->push_back(temp);
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

int readFileAll(TByte** out_pdata, size_t* out_dataSize, const char* fileName)
{
	size_t dataSize;
	hpatch_StreamPos_t  file_length = 0;
	FILE*               file = 0;
	assert((*out_pdata) == 0);
	if (!fileOpenForRead(fileName, &file, &file_length)) _file_error(file);

	dataSize = (size_t)file_length;
	if (dataSize != file_length) _file_error(file);
	*out_pdata = (TByte*)malloc(dataSize);
	if (*out_pdata == 0) _file_error(file);
	*out_dataSize = dataSize;

	if (!fileRead(file, *out_pdata, (*out_pdata) + dataSize)) _file_error(file);
	return fileClose(&file);
}

int writeFileAll(const TByte* pdata, size_t dataSize, const char* outFileName)
{
	FILE*   file = 0;
	if (!fileOpenForCreateOrReWrite(outFileName, &file)) _file_error(file);
	if (!fileWrite(file, pdata, pdata + dataSize)) _file_error(file);
	return fileClose(&file);
}

// Diff E:\Download\old E:\Download\new E:\Download\patch
bool DiffThread(const char* oldDir, const char* newDir, const char* outDir)
{
	char oldFile[MAX_PATH], newFile[MAX_PATH], patch[MAX_PATH], temp[MAX_PATH];

	if (_access(outDir, 0) != 0)
		_mkdir(outDir);

	std::vector<std::string> filelist;
	SearchFile(newDir, "*.*", &filelist);

  int nID = JSONManager::GetInstance().Create("{}");
  if (nID == -1)
  {
    LogManager::GetInstance().Log("Updater", LogManager::LOGINFO, "创建json失败");
    return false;
  }

	auto it = filelist.begin();
	while (it != filelist.end())
	{
		sprintf_s(oldFile, "%s\\%s", oldDir, it->c_str());
		sprintf_s(newFile, "%s\\%s", newDir, it->c_str());

		TByte* oldData = 0;  size_t oldDataSize = 0;
		TByte* newData = 0;  size_t newDataSize = 0;
		TByte* diffData = 0; size_t diffDataSize = 0;

		if (!readFileAll(&oldData, &oldDataSize, oldFile)) return false;
		if (!readFileAll(&newData, &newDataSize, newFile)) return false;

		std::vector<TByte> outDiffData;
		create_compressed_diff(newData, newData + newDataSize, oldData, oldData + oldDataSize, outDiffData, &zlibCompressPlugin, 4);

		sprintf_s(patch, "%s\\%s.patch", outDir, it->c_str());
		if (!writeFileAll(outDiffData.data(), outDiffData.size(), patch)) return false;

		_free_mem(diffData);
		_free_mem(newData);
		_free_mem(oldData);

    JSONManager::GetInstance().Set(nID, "old", it->c_str());
    JSONManager::GetInstance().Set(nID, "new", it->c_str());
		sprintf_s(temp, "%s.patch", it->c_str());
    JSONManager::GetInstance().Set(nID, "patch", temp);
    JSONManager::GetInstance().Set(nID, "md5", Md5::CalcFile(oldFile).c_str());

		++it;
	}

	sprintf_s(temp, "%s\\filelist.json", outDir);
  JSONManager::GetInstance().Write(temp);

	return true;
}

// Patch E:\Download\old E:\Download\new
bool PatchThread(const char* inDir, const char* outDir)
{
	TFileStreamInput oldData;
	TFileStreamOutput newData;
	TFileStreamInput diffData;
	hpatch_StreamPos_t savedNewSize = 0;
	hpatch_TStreamInput* poldData = &oldData.base;
	TByte* temp_cache = 0;
	size_t temp_cache_size;
	char oldFile[MAX_PATH], newFile[MAX_PATH], patch[MAX_PATH];

	if (_access(outDir, 0) != 0)
		_mkdir(outDir);

  int nID = JSONManager::GetInstance().Read("%s\\filelist.json");

	for (int i = 0; i < JSONManager::GetInstance().Count(nID, "old"); ++i)
	{
		sprintf_s(oldFile, "%s\\%s", inDir, JSONManager::GetInstance().GetString(nID, "old"));
		sprintf_s(newFile, "%s\\%s", outDir, JSONManager::GetInstance().GetString(nID, "new"));
		sprintf_s(patch, "%s\\Update\\%s", inDir, JSONManager::GetInstance().GetString(nID, "patch"));

		TFileStreamInput_init(&oldData);
		TFileStreamInput_init(&diffData);
		TFileStreamOutput_init(&newData);

		if (!TFileStreamInput_open(&oldData, oldFile)) return false;
		if (!TFileStreamInput_open(&diffData, patch)) return false;

		hpatch_compressedDiffInfo diffInfo;
		getCompressedDiffInfo(&diffInfo, &diffData.base);
		savedNewSize = diffInfo.newDataSize;

		TFileStreamOutput_open(&newData, newFile, savedNewSize);

		temp_cache_size = (size_t)(poldData->streamSize + kPatchCacheSize_bestmin);
		if (temp_cache_size != poldData->streamSize + kPatchCacheSize_bestmin)
			temp_cache_size = kPatchCacheSize_bestmax;
		while (!temp_cache)
		{
			temp_cache = (TByte*)malloc(temp_cache_size);
			if ((!temp_cache) && (temp_cache_size >= kPatchCacheSize_min * 2)) temp_cache_size >>= 1;
		}
		patch_decompress_with_cache(&newData.base, poldData, &diffData.base, &zlibDecompressPlugin, temp_cache, temp_cache + temp_cache_size);

		TFileStreamOutput_close(&newData);
		TFileStreamInput_close(&diffData);
		TFileStreamInput_close(&oldData);

		_free_mem(temp_cache);

		if (JSONManager::GetInstance().GetString(nID, "md5") != Md5::CalcFile(newFile))
			Help::GetInstance().GetInstance().Log("文件%s效验失败", newFile);
	}

	return true;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	if (Help::GetInstance().isStarted(szWindowClass)) return 0;

	Help::GetInstance().GetInstance().Log("初始化Update");

    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	int argc;
	LPWSTR *argv = 0;
	argv = CommandLineToArgvW(lpCmdLine, &argc);
	if (!argv || (wcscmp(argv[0], L"Update") == 0))
	{
		Help::GetInstance().GetInstance().Log("开始解压缩...");

		std::thread t([=]()->void
		{
			wchar_t szLauncher[MAX_PATH];
			swprintf_s(szLauncher, L"%s\\%s", argv[2], argv[3]);
			if (!Help::GetInstance().KillProcess(argv[3], argv[2]))
			{
        Help::GetInstance().Log("关闭进程 %s 失败", Help::GetInstance().WideToMulit(szLauncher).c_str());
        MessageBox(0, L"自动更新失败，请手动解压update目录下的压缩包覆盖原文件或下载最新安装包进行安装。", L"错误", MB_OK);
				exit(0);
			}

			if (Help::GetInstance().ExtractZip(Help::GetInstance().WideToMulit(argv[1]).c_str(), Help::GetInstance().WideToMulit(argv[2]).c_str()))
			{
				Sleep(500);
				LocalFree(argv);

				Help::GetInstance().Log("解压缩完毕，启动 %s", Help::GetInstance().WideToMulit(szLauncher).c_str());
				Help::GetInstance().ShellExe(szLauncher);

				exit(0);
			}
			else
			{
				LocalFree(argv);
				Help::GetInstance().GetInstance().Log("解压缩失败");
				MessageBox(0, L"自动更新失败，请手动解压update目录下的压缩包覆盖原文件或下载最新安装包进行安装。", L"错误", MB_OK);
				exit(0);
			}
		});
		t.join();
	}
	else if (!argv || (wcscmp(argv[0], L"Diff") == 0))
	{
		Help::GetInstance().GetInstance().Log("开始创建补丁...");

		char s1[MAX_PATH], s2[MAX_PATH], s3[MAX_PATH];

		sprintf_s(s1, "%s", Help::GetInstance().WideToMulit(argv[1]).c_str());
		sprintf_s(s2, "%s", Help::GetInstance().WideToMulit(argv[2]).c_str());
		sprintf_s(s3, "%s", Help::GetInstance().WideToMulit(argv[3]).c_str());

		auto result = std::async(std::launch::async, &DiffThread, s1, s2, s3);
		if (result.get())
		{
			Help::GetInstance().Log("创建补丁完毕");
			MessageBox(0, L"创建补丁完毕", szTitle, MB_OK);
			LocalFree(argv);
			return 0;
		}

		Help::GetInstance().Log("创建补丁失败");
	}
	else if (!argv || (wcscmp(argv[0], L"Patch") == 0))
	{
		Help::GetInstance().Log("开始应用补丁...");

		char s1[MAX_PATH], s2[MAX_PATH];

		sprintf_s(s1, "%s", Help::GetInstance().WideToMulit(argv[1]).c_str());
		sprintf_s(s2, "%s", Help::GetInstance().WideToMulit(argv[2]).c_str());

		auto result = std::async(std::launch::async, &PatchThread, s1, s2);
		if (result.get())
		{
			Help::GetInstance().Log("应用补丁完毕");
			MessageBox(0, L"应用补丁完毕", szTitle, MB_OK);
			LocalFree(argv);
			return 0;
		}

		Help::GetInstance().Log("应用补丁失败");
	}
	else
	{
		Help::GetInstance().Log("缺少必须的启动参数");
	}
	LocalFree(argv);

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	Help::GetInstance().Log("关闭Update");

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_UPDATE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_UPDATE));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd) return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
