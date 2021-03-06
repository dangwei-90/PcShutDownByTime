
// PcShutDownByTimeDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "PcShutDownByTime.h"
#include "PcShutDownByTimeDlg.h"
#include "afxdialogex.h"
#include "Define.h"
#include "Win7ShellApi.h"
#include <commctrl.h>
#include <Shobjidl.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 自动关机FUNC
DWORD WINAPI ThreadShutDownFunc(LPVOID lpParam)
{
	CPcShutDownByTimeDlg *MyDlg = (CPcShutDownByTimeDlg*)lpParam;
	while (1)
	{
		Sleep(500);
		::PostMessage(MyDlg->m_hWnd, WM_DISPLAY_TIME, 0, 0);
	}

	return 0;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPcShutDownByTimeDlg 对话框



CPcShutDownByTimeDlg::CPcShutDownByTimeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PCSHUTDOWNBYTIME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bShutDown = 0;
}

void CPcShutDownByTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_CURRENTTIME, m_currenttime);
	DDX_Control(pDX, IDC_COMBO_HOUR, m_shutdownhour);
	DDX_Control(pDX, IDC_COMBO_MINIUTE, m_shutdownminiute);

}

BEGIN_MESSAGE_MAP(CPcShutDownByTimeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBUTTON_OK, &CPcShutDownByTimeDlg::OnBnClickedOk)
	ON_MESSAGE(WM_DISPLAY_TIME, OnDisplayTime)
END_MESSAGE_MAP()


// CPcShutDownByTimeDlg 消息处理程序

BOOL CPcShutDownByTimeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	for (int n = 0; n <= 23; n++)
	{
		CString csNum;
		csNum.Format("%02d", n);
		m_shutdownhour.InsertString(0, csNum.GetBuffer());
		csNum.ReleaseBuffer();
	}
	m_shutdownhour.SetWindowTextA("23");

	for (int m = 0; m <= 59; m++)
	{
		CString csNum;
		csNum.Format("%02d", m);
		m_shutdownminiute.InsertString(0, csNum.GetBuffer());
		csNum.ReleaseBuffer();
	}
	m_shutdownminiute.SetWindowTextA("30");

	// 启动判断关机线程
	HANDLE hThread;
	DWORD dwThreadId;
	hThread = CreateThread(NULL		// 默认安全属性  
		, NULL						// 默认堆栈大小  
		, ThreadShutDownFunc		// 线程入口地址  
		, this						//传递给线程函数的参数  
		, 0							// 指定线程立即运行  
		, &dwThreadId				//线程ID号  
	);

	// CloseHandle(hThread);   //关闭线程句柄，内核引用计数减一  

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPcShutDownByTimeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPcShutDownByTimeDlg::OnPaint()
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
HCURSOR CPcShutDownByTimeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPcShutDownByTimeDlg::OnBnClickedOk()
{
	if (m_bShutDown == TRUE)
	{
		m_shutdownhour.EnableWindow(TRUE);
		m_shutdownminiute.EnableWindow(TRUE);
		GetDlgItem(IDBUTTON_OK)->SetWindowTextA(_T("启动定时关机"));
		m_bShutDown = FALSE;
	}
	else
	{
		m_shutdownhour.EnableWindow(FALSE);
		m_shutdownminiute.EnableWindow(FALSE);
		GetDlgItem(IDBUTTON_OK)->SetWindowText(_T("退出定时关机"));
		m_bShutDown = TRUE;
	}

	ITaskbarList3 * pTaskbarList = NULL;
	CoInitialize(0);
	CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_ITaskbarList, (void **)&pTaskbarList);
	/*
	设置进度条的类型
	TBPF_NORMAL、TBPF_INDETERMINATE、TBPF_NOPROGRESS------蓝色
	TBPF_ERROR-------红色
	TBPF_PAUSED------黄色
	*/
	pTaskbarList->SetProgressState(this->GetSafeHwnd(), TBPF_PAUSED);
	/*
	设置进度条的位置，现在设为7/10
	*/
	pTaskbarList->SetProgressValue(this->GetSafeHwnd(), 7, 10);
}


LRESULT CPcShutDownByTimeDlg::OnDisplayTime(WPARAM wParam, LPARAM lParam)
{
	//更新Edit  
	CTime t = CTime::GetCurrentTime();	//获取系统日期
	int d = t.GetDay();					//获得几号
	int y = t.GetYear();				//获取年份
	int m = t.GetMonth();				//获取当前月份
	int h = t.GetHour();				//获取当前为几时
	int mm = t.GetMinute();				//获取分钟
	int s = t.GetSecond();				//获取秒
	int w = t.GetDayOfWeek();			//获取星期几，注意1为星期天，7为星期六

	CString csCurrentTime;
	csCurrentTime.Format("%04d/%02d/%02d %02d:%02d:%02d", y, m, d, h, mm, s);

	SetDlgItemText(IDC_EDIT_CURRENTTIME, csCurrentTime.GetBuffer());
	csCurrentTime.ReleaseBuffer();

	int nhour = 0, nminiute = 0;
	CString csHour;
	m_shutdownhour.GetWindowTextA(csHour);
	if (csHour.GetLength() == 0)
	{
		nhour = 25;
	}
	else
	{
		nhour = _ttoi(csHour);
	}
	
	CString csMiniute;
	m_shutdownminiute.GetWindowTextA(csMiniute);
	if (csMiniute.GetLength() == 0)
	{
		nminiute = 61;
	}
	else
	{
		nminiute = _ttoi(csMiniute);
	}

	if (h == nhour && mm == nminiute && m_bShutDown == TRUE)
	{
		system("shutdown -s -t 0");

	}

	return 0;
}