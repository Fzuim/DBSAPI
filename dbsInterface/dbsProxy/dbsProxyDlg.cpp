
// dbsProxyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "dbsProxy.h"
#include "dbsProxyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CdbsProxyDlg 对话框



CdbsProxyDlg::CdbsProxyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdbsProxyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdbsProxyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdbsProxyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_TEST, &CdbsProxyDlg::OnBnClickedBtnTest)
END_MESSAGE_MAP()


// CdbsProxyDlg 消息处理程序

BOOL CdbsProxyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	CPubFunc::GetModulePath(NULL, g_Invoke_pi.m_tszCurExePath);
	m_bDBInterfaceInited = InitDBInterface();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CdbsProxyDlg::OnPaint()
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
HCURSOR CdbsProxyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#ifdef _WIN64
#define DLLNAME _T("dbsInterface64.dll")
#else
#define DLLNAME _T("dbsInterface.dll")
#endif // _WIN64


BOOL CdbsProxyDlg::InitDBInterface()
{
	BOOL bRet = FALSE;

	TCHAR tszDLLPath[MAX_PATH_EXP] = { 0 };
	_stprintf_s(tszDLLPath, _T("%s\\\%s"), g_Invoke_pi.m_tszCurExePath, DLLNAME);

	m_hDBInterface = ::LoadLibrary(tszDLLPath);
	if (NULL != m_hDBInterface)
	{
		m_pDLLTest = (PDLLTEST)::GetProcAddress(m_hDBInterface, "DLLTest");
		if (NULL != m_pDLLTest)
		{
			bRet = TRUE;
		}
	}
	else
	{
		CLogModule::Odprintf(_T("~!@ %s LoadLibrary[%s] Failed[%d]"), __FUNCTIONW__, tszDLLPath, GetLastError());
	}

	return bRet;
}

void CdbsProxyDlg::OnBnClickedBtnTest()
{
	// TODO:  在此添加控件通知处理程序代码
	if (NULL != m_pDLLTest)
	{
		m_pDLLTest(1);
	}
}

