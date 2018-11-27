
// MonitorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Monitor.h"
#include "MonitorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_MS             (500)
#define FIRST_CHECK_DELAY_S  (5)
#define NORMAL_CHECK_DELAY_S (10)
#define RESTART_DELAY_S      (10*24*60*60)

// CMonitorDlg 对话框

CMonitorDlg::CMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMonitorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_cnt = (NORMAL_CHECK_DELAY_S - FIRST_CHECK_DELAY_S)*(1000/TIMER_MS);

	HMODULE hUser32 = GetModuleHandle("USER32.DLL");
	if (hUser32)
	{
		m_IsHungNT  = (HUNG_FUNNT)GetProcAddress(hUser32, "IsHungAppWindow");
	}
}

void CMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICT_CTRL, m_pict_ctrl);
}

BEGIN_MESSAGE_MAP(CMonitorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCPAINT()
	ON_MESSAGE(WM_SHOWTASK, &CMonitorDlg::OnShowtask)
	ON_COMMAND(ID_MENU_EXIT, &CMonitorDlg::OnMenuExit)
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CMonitorDlg 消息处理程序

BOOL CMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//使对话框不在系统任务栏中显示
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	//托盘中显示图标
	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd = this->m_hWnd;
	m_nid.uID = IDR_MAINFRAME;
	m_nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	m_nid.uCallbackMessage = WM_SHOWTASK;//自定义的消息名称
	m_nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	strcpy(m_nid.szTip,"采集软件监控程序"); //信息提示条
	Shell_NotifyIcon(NIM_ADD, &m_nid); //在托盘区添加图标

	SetTimer(0, TIMER_MS, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMonitorDlg::OnNcPaint()
{
	static int i = 3;

	if(i > 0)
	{
		i --;
		ShowWindow(SW_HIDE);
	}
	else
	{
		CDialog::OnNcPaint();
	}
}

afx_msg LRESULT CMonitorDlg::OnShowtask(WPARAM wParam, LPARAM lParam)
{
	if(wParam != IDR_MAINFRAME)
	{
		return 1;
	}

	switch(lParam)
	{
	case WM_RBUTTONUP://右键起来时弹出快捷菜单
		{
			CPoint point;
			CMenu menu;
			CMenu *pPopup;

			GetCursorPos(&point);

			menu.LoadMenu(IDR_MENU_SYS);
			pPopup=menu.GetSubMenu(0); 

			this->SetForegroundWindow(); 
			pPopup->TrackPopupMenu(TPM_RIGHTALIGN|TPM_RIGHTBUTTON,point.x, point.y,this);
		} 
		break;

	case WM_LBUTTONUP:
		{
			this->ShowWindow(SW_SHOW);
			this->SetForegroundWindow(); 
		} 
		break;

	default: 
		break;
	}

	return 0;
}

void CMonitorDlg::OnMenuExit()
{
	// TODO: Add your command handler code here
	CDialog::OnCancel();
}

void CMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
	if (nID == SC_CLOSE)  
	{  
		ShowWindow(SW_HIDE);  
	} 
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CMonitorDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_pict_ctrl.IsWindowVisible())
	{
		m_pict_ctrl.ShowWindow(SW_HIDE);
	}
	else
	{
		m_pict_ctrl.ShowWindow(SW_SHOW);
	}

	if (++m_cnt >= NORMAL_CHECK_DELAY_S*(1000/TIMER_MS))
	{
		WatchDog();
		m_cnt = 0;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CMonitorDlg::LogDebugFile(CString &Content)
{
	CTime time = CTime::GetCurrentTime();
	CFile TmpFile;
	CString FileName;
	FileName.Format(".\\monitor.log");

	if(!TmpFile.Open(FileName,
		CFile :: typeBinary | CFile :: shareDenyNone |CFile :: modeWrite,
		NULL))
	{
		if(!TmpFile.Open(FileName,
			CFile :: typeBinary | CFile :: shareDenyNone |CFile :: modeCreate,
			NULL))
			return;
		TmpFile.Close();
		TmpFile.Open(FileName,
			CFile :: typeBinary | CFile :: shareDenyNone |CFile :: modeWrite,
			NULL);
	}

	char TmpString[128];
	sprintf(TmpString, "[%04d:%02d:%02d %02d:%02d:%02d] ", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	TmpFile.SeekToEnd();
	TmpFile.Write(TmpString, strlen(TmpString));
	TmpFile.Write(Content, Content.GetLength());
	TmpFile.Write("\r\n", 2);
	TmpFile.Close();
}

bool CMonitorDlg::GetProcIDByExeName(DWORD &ProcessID, char* ExeName)
{
	PROCESSENTRY32 ProcEntry;
	HANDLE hSnapShot;
	bool ret = false;
	bool is_first = true;
	BOOL hRes = false;

	ProcEntry.dwSize =sizeof(PROCESSENTRY32);

	hSnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	if(hSnapShot)
	{
		while(1)
		{
			if(is_first)
			{
				hRes = Process32First(hSnapShot, &ProcEntry);
				is_first = false;
			}
			else
			{
				hRes = Process32Next(hSnapShot, &ProcEntry);
			}

			if(hRes == false)
			{
				break;
			}

			if(!strcmp(ExeName, ProcEntry.szExeFile))
			{
				ProcessID = ProcEntry.th32ProcessID;
				ret = true;
				break;
			}
		}

		CloseHandle(hSnapShot);
	}

	return ret;
}

HWND CMonitorDlg::GetHwndByPid(DWORD ProcessID)
{
    HWND h = ::GetTopWindow(0);

    while (h)
    {
        DWORD pid = 0;
        DWORD dwTheardId = ::GetWindowThreadProcessId(h, &pid);

        if (dwTheardId != 0)
        {
            if (pid == ProcessID)
            {
                // here h is the handle to the window
                return h;
            }
        }

        h = ::GetNextWindow( h , GW_HWNDNEXT);
    }

	return NULL;
}

bool CMonitorDlg::IsTimeEqual(FILETIME &A, FILETIME &B)
{
	return ((A.dwHighDateTime == B.dwHighDateTime) && (A.dwLowDateTime == B.dwLowDateTime));
}

void CMonitorDlg::WatchDog(void)
{
	if (!ProcIsExist())
	{
		StartProc();
	}
	else
	{
		if (!ProcIsOK())
		{
			StartProc();
		}
	}
}

bool CMonitorDlg::ProcIsExist(void)
{
	bool ret = false;

	if (GetProcIDByExeName(m_ProcID, "col_app.exe"))
	{
		ret = true;
	}

	return ret;
}

bool CMonitorDlg::ProcIsOK(void)
{
	bool ret = false;
	bool need_end;
	HANDLE hProc;
	BOOL hRes;
	DWORD ExitCode;
	FILETIME CreationTime, ExitTime, KernelTime, UserTime;
	SYSTEMTIME SystemTime;
	CTime TmpTime, CurTime;

	hProc = OpenProcess(PROCESS_ALL_ACCESS, TRUE, m_ProcID);
	if(hProc)
	{	
		need_end = true;

		m_hwnd = GetHwndByPid(m_ProcID);
		if (0 != m_hwnd)
		{
			//if (!m_IsHungNT(m_hwnd))
			if (!IsHungAppWindow(m_hwnd))
			{
				hRes = GetProcessTimes(hProc, &CreationTime, &ExitTime, &KernelTime, &UserTime);
				if (hRes)
				{
					if(FileTimeToSystemTime(&CreationTime, &SystemTime))
					{
						TmpTime = CTime(SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);
						CurTime = CTime::GetCurrentTime();

						if(CurTime - TmpTime < RESTART_DELAY_S)
						{
							need_end = false;
							ret = true;
						}
					}
				}
			}
		}

		if (need_end)
		{
			CString TmpS;
			TmpS.Format("TerminateProcess\n");
			LogDebugFile(TmpS);

			hRes = TerminateProcess(hProc, 0);
			if (hRes)
			{
				//等待退出
				while(GetExitCodeProcess(hProc, &ExitCode))
				{
					if(ExitCode == STILL_ACTIVE)
					{
						Sleep(1000);
					}
					else
					{
						break;
					}
				}
			}
		}

		CloseHandle(hProc);
		hProc = NULL;
	}

	return ret;
}

void CMonitorDlg::StartProc(void)
{
	STARTUPINFO StartUpInfo;
	PROCESS_INFORMATION ProcessInfo;

	CString TmpS;
	TmpS.Format("CreateProcess\n");
	LogDebugFile(TmpS);

	ZeroMemory(&StartUpInfo, sizeof(STARTUPINFO));
	StartUpInfo.cb = sizeof (STARTUPINFO);
	StartUpInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartUpInfo.wShowWindow = SW_HIDE;

	ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));

	CreateProcess(".\\col_app.exe",
						NULL,
						NULL,
						NULL,
						FALSE,
						NORMAL_PRIORITY_CLASS,
						NULL,
						NULL,
						&StartUpInfo,
						&ProcessInfo);

	CloseHandle(ProcessInfo.hProcess);
	CloseHandle(ProcessInfo.hThread);
}
