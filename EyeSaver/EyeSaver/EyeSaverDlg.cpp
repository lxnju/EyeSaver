
// EyeSaverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EyeSaver.h"
#include "EyeSaverDlg.h"
#include "afxdialogex.h"
#include "..\KeyboardMouseHookDll\KeyboardMouseHookDll.h"
#include "ConfigScreensaver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CEyeSaverDlg dialog



CEyeSaverDlg::CEyeSaverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEyeSaverDlg::IDD, pParent)
	, m_interval(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEyeSaverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_interval);
	DDV_MinMaxInt(pDX, m_interval, 0, 999);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressCtrl);
	DDX_Control(pDX, IDC_BUTTON1, m_buttonStart);
	DDX_Control(pDX, IDC_BUTTON2, m_buttonStop);
}

BEGIN_MESSAGE_MAP(CEyeSaverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CEyeSaverDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CEyeSaverDlg::OnBnClickedButton2)
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER_NOTIFYICON, OnNotifyMsg)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CEyeSaverDlg message handlers

BOOL CEyeSaverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CConfigScreensaver css;
	
	if (! css.ConfigBubble())
	{
		// warn user that screen saver is not configured and exit.
		MessageBox(L"Faied to configure your Screen Saver setting, please configure your Screen Saver to bubble and restart this application.");
	}
	// set system tray icon
	m_notify.cbSize = sizeof NOTIFYICONDATA;
	m_notify.hWnd = this->m_hWnd;	
	m_notify.uID = IDR_MAINFRAME;
	m_notify.hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy_s(m_notify.szTip, L"Eye Saver");
	m_notify.uCallbackMessage = WM_USER_NOTIFYICON;
	m_notify.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP; //OK,下面就是托盘产生了. 
	Shell_NotifyIcon(NIM_ADD,&m_notify);

	m_elapsedMinutes = 0;
	m_interval = 60;
	m_progressCtrl.SetRange(0, 100);
	m_progressCtrl.SetPos(0);
	m_buttonStart.EnableWindow();
	m_buttonStop.EnableWindow(FALSE);
	
	UpdateData(0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEyeSaverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEyeSaverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEyeSaverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEyeSaverDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData(1);

	// timer to start screen saver
	SetTimer(1, m_interval * 60 * 1000, NULL);
	// timer to update the progress bar
	SetTimer(2, 60 * 1000, NULL);

	// 设置pos为1是为了不让progress bar一开始显示为空的，那样比较不好看。
	m_progressCtrl.SetPos(1);
	m_buttonStart.EnableWindow(FALSE);
	m_buttonStop.EnableWindow(TRUE);
}

void CEyeSaverDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
	KillTimer(2);
	KillTimer(3);
	KillTimer(4);
	m_elapsedMinutes = 0;
	m_buttonStart.EnableWindow(TRUE);
	m_buttonStop.EnableWindow(FALSE);
	m_progressCtrl.SetPos(0);
}

void CEyeSaverDlg::OnTimer(UINT_PTR nEventId)
{
	switch(nEventId)  
    {  
     case 1:  
		 KillTimer(1);
		 // before start screen saver,
		 // wait for the progress bar to update
		 SetTimer(4, 800, NULL);
         
         break;  
     case 2:  
         // update the progress bar  
         KillTimer(2); 
		 m_elapsedMinutes++;
		 if (m_elapsedMinutes <= m_interval)
		 {
			int newpos = m_elapsedMinutes * 100 / m_interval;
			// 不让进度条小于1导致后退
			m_progressCtrl.SetPos(newpos > 1 ? newpos : 1);
			SetTimer(2, 60 * 1000, NULL);
		 }
         break;
	 case 3:
		 {
			KillTimer(3);
			// wait for the screen saver to be closed by user
			BOOL bScreenSaverRunning = TRUE;
			SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &bScreenSaverRunning, 0);
			if (bScreenSaverRunning == TRUE)
			{
				SetTimer(3, 5 * 1000, NULL);
			}
			else
			{
				// screen saver not running could be because of
				// user locks the screen, so need to unload hook
				RemoveKeyboardMouseHook();

				// start another round of loop
				m_elapsedMinutes = 0;
				
				OnBnClickedButton1();

			}
			break;
		 }
	 case 4:
		 // start screen saver
		 KillTimer(4);
		 DoBubbleScreenSaver(); 
		 SetTimer(3, 5 * 1000, NULL);
	}
}

void CEyeSaverDlg::DoBubbleScreenSaver()
{
	// all the hooks will be unloaded by windows if the application exits.
	SetKeyboardMouseHook();
	::PostMessage(::GetDesktopWindow(), WM_SYSCOMMAND, SC_SCREENSAVE, 0);
}

LRESULT CEyeSaverDlg::OnNotifyMsg(WPARAM wparam,LPARAM lparam)
{
	//wParam接收的是图标的ID，而lParam接收的是鼠标的行为   
	if(wparam!=IDR_MAINFRAME)   
		return 1;   
    switch(lparam)   
    {   
    case WM_RBUTTONUP:
	case WM_LBUTTONUP:
    case WM_LBUTTONDBLCLK:   
		ShowWindow(SW_SHOWNORMAL);//简单的显示主窗口完事儿   
		SetForegroundWindow();
		break;
	default:
		break;
    }    
	return 0;
}

void CEyeSaverDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(nType==SIZE_MINIMIZED)
	{
		ShowWindow(SW_HIDE);
	}
}
