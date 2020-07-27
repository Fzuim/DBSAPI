
// dbsProxyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "dbsProxy.h"
#include "dbsProxyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CdbsProxyDlg �Ի���



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


// CdbsProxyDlg ��Ϣ�������

BOOL CdbsProxyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	CPubFunc::GetModulePath(NULL, g_Invoke_pi.m_tszCurExePath);
	m_bDBInterfaceInited = InitDBInterface();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CdbsProxyDlg::OnPaint()
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (NULL != m_pDLLTest)
	{
		m_pDLLTest(1);
	}
}

