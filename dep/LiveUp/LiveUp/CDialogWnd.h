#pragma once

#include "resource.h"
#include "UIlib.h"

class CDialogWnd : public CWindowWnd, public INotifyUI, public CDwm, public CDPI
{
public:
	CDialogWnd(const wchar_t* szInfo, const wchar_t* szOldVersion, const wchar_t* szNewVersion);

	LPCTSTR GetWindowClassName() const { return _T("UILiveUpWnd"); };
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
	void Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	HWND GetHWND() { return m_hWnd; }

private:
	CPaintManagerUI m_pm;

	const wchar_t* m_szInfo;
	const wchar_t* m_szOldVersion;
	const wchar_t* m_szNewVersion;
};

