
// USBCCtrlPanelDlg.h : ͷ�ļ�
//

#pragma once
#include "LeoURLProtocol.h"

// CUSBCCtrlPanelDlg �Ի���
class CUSBCCtrlPanelDlg : public CDialogEx
{
// ����
public:
	CUSBCCtrlPanelDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_USBCCTRLPANEL_DIALOG };

	void InitParameter();

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CLeoURLProtocol m_URLProtocol;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnUnreg();
	afx_msg void OnBnClickedBtnReg();
	afx_msg void OnBnClickedBtnPost2();
	afx_msg void OnBnClickedBtnPost();
	afx_msg void OnBnClickedBtnSelect();
	UINT m_uiPort;
};
