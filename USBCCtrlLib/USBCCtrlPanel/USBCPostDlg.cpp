// USBCPostDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "USBCCtrlPanel.h"
#include "USBCPostDlg.h"
#include "afxdialogex.h"

#include "USBCLib.h"

#define TIMER_USER			0x1000
#define TIMER_EXIT			(TIMER_USER+1)
#define TIMER_PROGRESS		(TIMER_USER+2)

const int TotalTime=20000;
const int ProgressRange = 100;
const int ProgressStepTime = TotalTime/ProgressRange;

static volatile int ProgressPos=0;

// CUSBCPostDlg �Ի���

IMPLEMENT_DYNAMIC(CUSBCPostDlg, CDialog)

CUSBCPostDlg::CUSBCPostDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUSBCPostDlg::IDD, pParent)
{
	InitUSBCLib();
	
	m_DevSize=0;
}

CUSBCPostDlg::~CUSBCPostDlg()
{
	UninitUSBCLib();
}

void CUSBCPostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_CTRL, m_ProgressCtrl);
}

BEGIN_MESSAGE_MAP(CUSBCPostDlg, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CUSBCPostDlg ��Ϣ�������


BOOL CUSBCPostDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText("�Լ�USB��Ⱥ�豸");
	ModifyStyle(WS_CAPTION, NULL, SWP_DRAWFRAME );
	SetDlgItemText(IDC_STATIC_TITLE,"�Լ�USB��Ⱥ�豸" );
	SetDlgItemText(IDC_STATIC_LOG,"��Ϣ�� ׼���Լ�USB��Ⱥ�豸..." );

	m_ProgressCtrl.SetRange(0, 100);  
    m_ProgressCtrl.SetPos(ProgressPos);

	SetTimer(TIMER_EXIT, TotalTime, NULL);
	SetTimer(TIMER_PROGRESS, ProgressStepTime, NULL);
	
	char res=dllRunUSBCLibPost();
	if(res>=0)
	{
		ProgressPos=100;
		dllGetUSBCDevList(m_ComNOList,m_DevIDList,&m_DevSize);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}


BOOL CUSBCPostDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam) 
		{
			case VK_RETURN: //�س�
				return TRUE;
			case VK_ESCAPE: //ESC
				return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CUSBCPostDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(nIDEvent>TIMER_USER)
	{
		switch(nIDEvent) 
		{
			case TIMER_EXIT:
				KillTimer(TIMER_EXIT);
				KillTimer(TIMER_PROGRESS);
				SetDlgItemText(IDC_STATIC_LOG,"���� ������ʱ�������˳�" );
				MessageBox("������ʱ�������˳�","����",MB_ICONEXCLAMATION );
				OnCancel();
				break;

			case TIMER_PROGRESS:
				KillTimer(TIMER_PROGRESS);
				if(ProgressPos++<ProgressRange*0.9)
				{
					m_ProgressCtrl.SetPos(ProgressPos);
					SetTimer(TIMER_PROGRESS,ProgressStepTime,NULL);
				}
				else if(ProgressPos>=100)
				{
					KillTimer(TIMER_EXIT);
					SetDlgItemText(IDC_STATIC_LOG,"�����ɹ�" );
					Sleep(1000);
					OnOK();
				}

				break;
		}
	}

	CDialog::OnTimer(nIDEvent);
}
