
// USBCCtrlPanel.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUSBCCtrlPanelApp:
// �йش����ʵ�֣������ USBCCtrlPanel.cpp
//

class CUSBCCtrlPanelApp : public CWinApp
{
public:
	CUSBCCtrlPanelApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUSBCCtrlPanelApp theApp;