
// USBCCtrlPanel.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "USBCCtrlPanel.h"

#include "USBCCtrlPanelDlg.h"
#include "USBCPostDlg.h"
#include "USBCSelectDlg.h"

#include "LeoUrlParser.h"

BYTE SendBuffer[4096]={0};
BYTE RecvBuffer[256]={0};

char gFileExe[MAX_PATH]={0};
char gFilePath[MAX_PATH]={0};


// CUSBCCtrlPanelApp

BEGIN_MESSAGE_MAP(CUSBCCtrlPanelApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CUSBCCtrlPanelApp ����

CUSBCCtrlPanelApp::CUSBCCtrlPanelApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CUSBCCtrlPanelApp ����

CUSBCCtrlPanelApp theApp;


// CUSBCCtrlPanelApp ��ʼ��
BOOL CUSBCCtrlPanelApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	
#ifdef _DEBUG  
    //AllocConsole();  
#endif

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	HMODULE module = GetModuleHandle(0);  
	GetModuleFileName(module, gFileExe, MAX_PATH); 
	CString csFullPath(gFileExe); 
	int nPos = csFullPath.ReverseFind( _T('\\') ); 
	strcpy_s(gFilePath,csFullPath.Left( nPos ).GetBuffer());

	CString strParam = AfxGetApp()->m_lpCmdLine;
	strParam=strParam.MakeLower();
	_DEBUG_PRINTF("%s\r\n", strParam);

	INT_PTR nResponse = NULL;
	CParser* pUrlParser = new CParser(strParam.GetBuffer());

	_DEBUG_PRINTF("HELLO HELL!");

	if(strcmp(pUrlParser->m_Path.c_str(),"post")==0)
	{
		CUSBCPostDlg dlg;

		m_pMainWnd = &dlg;
		nResponse= dlg.DoModal();
	}
	else if(strcmp(pUrlParser->m_Path.c_str(),"select")==0)
	{
		CUSBCSelectDlg dlg;

		unsigned char ucFoundDev=-1;
		unsigned char ucFoundPort=-1;

		for(int idx=0; idx<pUrlParser->m_QueryLength; idx++)
		{
			if(strcmp(pUrlParser->m_QueryParam[idx].QueryName.c_str(),"dev")==0)
			{
				dlg.m_ucDevID=atoi(pUrlParser->m_QueryParam[idx].QueryValue.c_str());
				ucFoundDev=0;
				if(dlg.m_ucDevID>=0 && dlg.m_ucDevID<=7)
				{
					ucFoundDev=1;
				}
			}
			else if(strcmp(pUrlParser->m_QueryParam[idx].QueryName.c_str(),"port")==0)
			{
				dlg.m_uiPortNO=atoi(pUrlParser->m_QueryParam[idx].QueryValue.c_str());
				ucFoundPort=0;
				if(dlg.m_uiPortNO>=0 && dlg.m_uiPortNO<=192)
				{
					ucFoundPort=1;
				}
			}
		}
		if(ucFoundDev==1 && ucFoundPort==1)
		{
			m_pMainWnd = &dlg;
			nResponse= dlg.DoModal();
		}
		else
		{
			if(ucFoundDev<0 || ucFoundPort<0)
			{
				MessageBox(NULL,"�밴 <leowebcmd://usbc/select?dev=x&port=y> ��ʽִ��Э�飡","����",MB_OK);
			}
			else if(ucFoundDev==0)
			{
				MessageBox(NULL,"��ѡ����豸ID��ų�����Χ��","����",MB_OK);
			}
			else if(ucFoundPort==0)
			{
				MessageBox(NULL,"��ѡ���USB��ų�����Χ��","����",MB_OK);
			}
			nResponse= 0;
		}
	}
#if _DEBUG==0
	else if(strcmp(pUrlParser->m_Path.c_str(),"ctrl")==0)
#endif
	{
		CUSBCCtrlPanelDlg dlg;
		m_pMainWnd = &dlg;
		nResponse= dlg.DoModal();
	}
#if _DEBUG==0
	else
	{
		CLeoURLProtocol URLProtocol;
		URLProtocol.setProtocolName("LeoWebCmd");
		URLProtocol.setAppPath(gFileExe);
		URLProtocol.DeleteCustomProtocol();
		URLProtocol.CreateCustomProtocol();
		nResponse = 0;
	}
#endif
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	delete pUrlParser;

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

BOOL CUSBCCtrlPanelApp::ExitInstance()
{
#ifdef _DEBUG  
    FreeConsole();  
#endif 

	return CWinApp::ExitInstance();
}