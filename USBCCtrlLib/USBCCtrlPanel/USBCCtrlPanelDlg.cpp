
// USBCCtrlPanelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "USBCCtrlPanel.h"
#include "USBCCtrlPanelDlg.h"
#include "afxdialogex.h"

#include "USBCPostDlg.h"
#include "USBCSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUSBCCtrlPanelDlg �Ի���



CUSBCCtrlPanelDlg::CUSBCCtrlPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUSBCCtrlPanelDlg::IDD, pParent)
	, m_uiPort(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUSBCCtrlPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, m_uiPort);
}

BEGIN_MESSAGE_MAP(CUSBCCtrlPanelDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_BTN_UNREG, &CUSBCCtrlPanelDlg::OnBnClickedBtnUnreg)
	ON_BN_CLICKED(ID_BTN_REG, &CUSBCCtrlPanelDlg::OnBnClickedBtnReg)
	ON_BN_CLICKED(IDC_BTN_POST, &CUSBCCtrlPanelDlg::OnBnClickedBtnPost)
	ON_BN_CLICKED(IDC_BTN_SELECT, &CUSBCCtrlPanelDlg::OnBnClickedBtnSelect)
END_MESSAGE_MAP()


// CUSBCCtrlPanelDlg ��Ϣ�������

BOOL CUSBCCtrlPanelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CUSBCCtrlPanelDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUSBCCtrlPanelDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CUSBCCtrlPanelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUSBCCtrlPanelDlg::OnBnClickedBtnReg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD errorCode = 0;

	m_URLProtocol.setProtocolName("LeoWebCmd");
	m_URLProtocol.setAppPath(gFileExe);

	if(m_URLProtocol.CreateCustomProtocol() != ERROR_SUCCESS)
	{
		MessageBox(m_URLProtocol.getErrorMsg().c_str());
	}
	else
	{
		MessageBox("Э��ע��ɹ���");
	}
}

void CUSBCCtrlPanelDlg::OnBnClickedBtnUnreg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD errorCode = 0;

	m_URLProtocol.setProtocolName("LeoWebCmd");
	m_URLProtocol.setAppPath(gFileExe);

	if(m_URLProtocol.DeleteCustomProtocol() != ERROR_SUCCESS)
	{
		MessageBox(m_URLProtocol.getErrorMsg().c_str());
	}
	else
	{
		MessageBox("Э��ɾ���ɹ���");
	}
}

void CUSBCCtrlPanelDlg::OnBnClickedBtnPost()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CUSBCPostDlg dlg;
	dlg.DoModal();
	
	char strTmp[256]={};
	if(dlg.m_DevSize>0)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST))->Clear();
		((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST))->ResetContent();
		for(int idx=0;idx<dlg.m_DevSize;idx++)
		{
			_itoa_s(dlg.m_DevIDList[idx], strTmp, 10);
			((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST))->AddString(strTmp);
			m_uiPort=0;
		}
		((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST))->SetWindowText(strTmp);
		UpdateData(false);
	}
}

void CUSBCCtrlPanelDlg::OnBnClickedBtnSelect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CString strTmp="";
	((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST))->GetWindowText(strTmp);
	int ucDevID = atoi(strTmp);
	if(ucDevID>=0 && ucDevID<256 && m_uiPort>=0 && m_uiPort<256)
	{
		CUSBCSelectDlg dlg;
		dlg.m_ucDevID=ucDevID;
		dlg.m_uiPortNO=m_uiPort;
		dlg.DoModal();
	}
	else
	{
		MessageBox("�豸��Ż�USB��Ŵ���","����",MB_OK);
	}
}
