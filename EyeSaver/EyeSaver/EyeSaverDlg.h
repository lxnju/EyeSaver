
// EyeSaverDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define WM_USER_NOTIFYICON (WM_USER + 1990) 

// CEyeSaverDlg dialog
class CEyeSaverDlg : public CDialogEx
{
// Construction
public:
	CEyeSaverDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EYESAVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	int m_interval;
	CProgressCtrl m_progressCtrl;
	CButton m_buttonStart;
	CButton m_buttonStop;
	int m_elapsedMinutes;
	NOTIFYICONDATA m_notify;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nEventId);
	afx_msg LRESULT OnNotifyMsg(WPARAM wparam,LPARAM lparam); 

	void DoBubbleScreenSaver();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
