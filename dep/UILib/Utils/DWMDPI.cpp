#include "stdafx.h"
#include "DWMDPI.h"

CDwm::CDwm()
{
	static HINSTANCE hDwmInstance = ::LoadLibrary(_T("dwmapi.dll"));
	if (hDwmInstance != NULL) {
		fnDwmEnableComposition = (FNDWMENABLECOMPOSITION) ::GetProcAddress(hDwmInstance, "DwmEnableComposition");
		fnDwmIsCompositionEnabled = (FNDWNISCOMPOSITIONENABLED) ::GetProcAddress(hDwmInstance, "DwmIsCompositionEnabled");
		fnDwmEnableBlurBehindWindow = (FNENABLEBLURBEHINDWINDOW) ::GetProcAddress(hDwmInstance, "DwmEnableBlurBehindWindow");
		fnDwmExtendFrameIntoClientArea = (FNDWMEXTENDFRAMEINTOCLIENTAREA) ::GetProcAddress(hDwmInstance, "DwmExtendFrameIntoClientArea");
		fnDwmSetWindowAttribute = (FNDWMSETWINDOWATTRIBUTE) ::GetProcAddress(hDwmInstance, "DwmSetWindowAttribute");
	}
	else {
		fnDwmEnableComposition = NULL;
		fnDwmIsCompositionEnabled = NULL;
		fnDwmEnableBlurBehindWindow = NULL;
		fnDwmExtendFrameIntoClientArea = NULL;
		fnDwmSetWindowAttribute = NULL;
	}
}

BOOL CDwm::IsCompositionEnabled() const
{
	HRESULT Hr = E_NOTIMPL;
	BOOL bRes = FALSE;
	if (fnDwmIsCompositionEnabled != NULL) Hr = fnDwmIsCompositionEnabled(&bRes);
	return SUCCEEDED(Hr) && bRes;
}

BOOL CDwm::EnableComposition(UINT fEnable)
{
	BOOL bRes = FALSE;
	if (fnDwmEnableComposition != NULL) bRes = SUCCEEDED(fnDwmEnableComposition(fEnable));
	return bRes;
}

BOOL CDwm::EnableBlurBehindWindow(HWND hWnd)
{
	BOOL bRes = FALSE;
	if (fnDwmEnableBlurBehindWindow != NULL) {
		DWM_BLURBEHIND bb = { 0 };
		bb.dwFlags = DWM_BB_ENABLE;
		bb.fEnable = TRUE;
		bRes = SUCCEEDED(fnDwmEnableBlurBehindWindow(hWnd, &bb));
	}
	return bRes;
}

BOOL CDwm::EnableBlurBehindWindow(HWND hWnd, CONST DWM_BLURBEHIND& bb)
{
	BOOL bRes = FALSE;
	if (fnDwmEnableBlurBehindWindow != NULL) {
		bRes = SUCCEEDED(fnDwmEnableBlurBehindWindow(hWnd, &bb));
	}
	return bRes;
}

BOOL CDwm::ExtendFrameIntoClientArea(HWND hWnd, CONST DWM_MARGINS& Margins)
{
	BOOL bRes = FALSE;
	if (fnDwmEnableComposition != NULL) bRes = SUCCEEDED(fnDwmExtendFrameIntoClientArea(hWnd, &Margins));
	return bRes;
}

BOOL CDwm::SetWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute)
{
	BOOL bRes = FALSE;
	if (fnDwmSetWindowAttribute != NULL) bRes = SUCCEEDED(fnDwmSetWindowAttribute(hwnd, dwAttribute, pvAttribute, cbAttribute));
	return bRes;
}

CDPI::CDPI()
{
	m_nScaleFactor = 0;
	m_nScaleFactorSDA = 0;
	m_Awareness = PROCESS_DPI_UNAWARE;

	static HINSTANCE hUser32Instance = ::LoadLibrary(_T("User32.dll"));
	static HINSTANCE hShcoreInstance = ::LoadLibrary(_T("Shcore.dll"));
	if (hUser32Instance != NULL) {
		fnSetProcessDPIAware = (FNSETPROCESSDPIAWARE) ::GetProcAddress(hUser32Instance, "SetProcessDPIAware");
	}
	else {
		fnSetProcessDPIAware = NULL;
	}

	if (hShcoreInstance != NULL) {
		fnSetProcessDpiAwareness = (FNSETPROCESSDPIAWARENESS) ::GetProcAddress(hShcoreInstance, "SetProcessDpiAwareness");
		fnGetDpiForMonitor = (FNGETDPIFORMONITOR) ::GetProcAddress(hShcoreInstance, "GetDpiForMonitor");
	}
	else {
		fnSetProcessDpiAwareness = NULL;
		fnGetDpiForMonitor = NULL;
	}

	if (fnGetDpiForMonitor != NULL) {
		UINT     dpix = 0, dpiy = 0;
		HRESULT  hr = E_FAIL;
		POINT pt = { 1, 1 };
		HMONITOR hMonitor = ::MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
		hr = fnGetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpix, &dpiy);
		SetScale(dpix);
	}
	else {
		UINT     dpix = 0;
		HDC hDC = ::GetDC(::GetDesktopWindow());
		dpix = GetDeviceCaps(hDC, LOGPIXELSX);
		::ReleaseDC(::GetDesktopWindow(), hDC);
		SetScale(dpix);
	}

	SetAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
}

PROCESS_DPI_AWARENESS CDPI::GetAwareness()
{
	return m_Awareness;
}

int CDPI::Scale(int x)
{
	if (m_Awareness == PROCESS_DPI_UNAWARE) return x;
	if (m_Awareness == PROCESS_SYSTEM_DPI_AWARE) return MulDiv(x, m_nScaleFactorSDA, 100);
	return MulDiv(x, m_nScaleFactor, 100); // PROCESS_PER_MONITOR_DPI_AWARE
}

UINT CDPI::GetScale()
{
	if (m_Awareness == PROCESS_DPI_UNAWARE) return 100;
	if (m_Awareness == PROCESS_SYSTEM_DPI_AWARE) return m_nScaleFactorSDA;
	return m_nScaleFactor;
}

void CDPI::ScaleRect(__inout RECT *pRect)
{
	pRect->left = Scale(pRect->left);
	pRect->right = Scale(pRect->right);
	pRect->top = Scale(pRect->top);
	pRect->bottom = Scale(pRect->bottom);
}

void CDPI::ScalePoint(__inout POINT *pPoint)
{
	pPoint->x = Scale(pPoint->x);
	pPoint->y = Scale(pPoint->y);
}

void CDPI::OnDPIChanged(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	SetScale(LOWORD(wParam));
	RECT* const prcNewWindow = (RECT*)lParam;
	::SetWindowPos(hWnd,
		NULL,
		prcNewWindow->left,
		prcNewWindow->top,
		prcNewWindow->right - prcNewWindow->left,
		prcNewWindow->bottom - prcNewWindow->top,
		SWP_NOZORDER | SWP_NOACTIVATE);
}

BOOL CDPI::SetAwareness(PROCESS_DPI_AWARENESS value)
{
	if (fnSetProcessDpiAwareness != NULL) {
		HRESULT Hr = E_NOTIMPL;
		Hr = fnSetProcessDpiAwareness(value);
		if (Hr == S_OK) {
			m_Awareness = value;
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	else {
		if (fnSetProcessDPIAware) {
			BOOL bRet = fnSetProcessDPIAware();
			if (bRet) m_Awareness = PROCESS_SYSTEM_DPI_AWARE;
			return bRet;
		}
	}
	return FALSE;
}

void CDPI::SetScale(__in UINT iDPI)
{
	m_nScaleFactor = MulDiv(iDPI, 100, 96);
	if (m_nScaleFactorSDA == 0) m_nScaleFactorSDA = m_nScaleFactor;
}