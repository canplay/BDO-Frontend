#include "stdafx.h"
#include "CDialogWnd.h"
#include <windows.h>

CDialogWnd::CDialogWnd(const wchar_t * szInfo, const wchar_t* szOldVersion, const wchar_t* szNewVersion)
{
	m_szInfo = szInfo;
	m_szOldVersion = szOldVersion;
	m_szNewVersion = szNewVersion;
}

void CDialogWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK) // 先判断消息类型，如果是单击
	{
		if (msg.pSender == m_pm.FindControl(L"Cancel"))
			exit(0);
	}
	else if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		CTextUI* UI_Info = (CTextUI*)m_pm.FindControl(L"Info");
		if (!UI_Info) return;
		UI_Info->SetText(m_szInfo);
		UI_Info->NeedParentUpdate();

		CTextUI* UI_OldVersion = (CTextUI*)m_pm.FindControl(L"OldVersion");
		if (!UI_OldVersion) return;
		UI_OldVersion->SetText(m_szOldVersion);

		CTextUI* UI_NewVersion = (CTextUI*)m_pm.FindControl(L"NewVersion");
		if (!UI_NewVersion) return;
		UI_NewVersion->SetText(m_szNewVersion);
	}
}

LRESULT CDialogWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;

	if (uMsg == WM_CREATE)
	{
		SetIcon(IDI_ICON1);

		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(L"skin.xml", (UINT)0, NULL, &m_pm);
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);

		DWMNCRENDERINGPOLICY ncrp = DWMNCRP_ENABLED;
		SetWindowAttribute(m_hWnd, DWMWA_TRANSITIONS_FORCEDISABLED, &ncrp, sizeof(ncrp));
		return lRes;
	}
	else if (uMsg == 0x099)
	{
		CProgressUI* UI_Progress = (CProgressUI*)m_pm.FindControl(L"Progress");
		if (!UI_Progress) return lRes;
		UI_Progress->SetValue(wParam);
		return lRes;
	}
	else if (uMsg == WM_DESTROY)
		::PostQuitMessage(0L);
	else if (uMsg == WM_NCACTIVATE)
		if (!::IsIconic(*this)) return (wParam == 0) ? TRUE : FALSE;

	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

