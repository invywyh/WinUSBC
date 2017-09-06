#pragma once
#include "afxcmn.h"

// CUSBCSelectDlg �Ի���

class CUSBCSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CUSBCSelectDlg)

public:
	CUSBCSelectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUSBCSelectDlg();

	unsigned int m_uiPortNO;
	unsigned char m_ucDevID;

// �Ի�������
	enum { IDD = IDD_USBCPROCDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CProgressCtrl m_ProgressCtrl;
};
