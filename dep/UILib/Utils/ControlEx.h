#pragma once

#include "UIlib.h"
using namespace UILib;

class UILIB_API ComputerExamineUI : public CContainerUI
{
public:
	ComputerExamineUI();
};

class UILIB_API CDialogBuilderCallbackEx : public IDialogBuilderCallback
{
public:
	CControlUI* CreateControl(LPCTSTR pstrClass);
};
