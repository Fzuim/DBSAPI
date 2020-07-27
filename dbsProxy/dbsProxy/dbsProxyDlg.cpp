// dbsProxyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "dbsProxy.h"
#include "dbsProxyDlg.h"
#include "IDBConnect.h"
#include "PushManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CdbsProxyDlg �Ի���




CdbsProxyDlg::CdbsProxyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CdbsProxyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pEnvMonEvent = NULL;
}

void CdbsProxyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdbsProxyDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_TESTORACLE, &CdbsProxyDlg::OnBnClickedBtnTestoracle)
	ON_BN_CLICKED(IDC_BTN_TESTMYSQL, &CdbsProxyDlg::OnBnClickedBtnTestmysql)
	ON_BN_CLICKED(IDC_BTN_ENVMON, &CdbsProxyDlg::OnBnClickedBtnEnvmon)
	ON_MESSAGE(WM_REFRESHMSG, DealRefreshMsg)
	ON_BN_CLICKED(IDC_BTN_STOP, &CdbsProxyDlg::OnBnClickedBtnStop)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

LRESULT CdbsProxyDlg::DealRefreshMsg(WPARAM v_wParam, LPARAM v_lParam)
{
	if (NULL != v_wParam)
	{
		PREFRESHMSG pRefreshMsg = (PREFRESHMSG)v_wParam;
		switch(pRefreshMsg->m_iMsgType)
		{
		case _ENVMSG_:
			{
				if (1 == pRefreshMsg->m_iMsgFlag)
					GetDlgItem(IDC_STATIC_EMS)->SetWindowText(pRefreshMsg->m_tszMsg);
			}
			break;
		default:
			break;
		}

		delete pRefreshMsg;
		pRefreshMsg = NULL;
	}
	return 0;
}

// CdbsProxyDlg ��Ϣ�������

BOOL CdbsProxyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	InitUI();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CdbsProxyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CdbsProxyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CdbsProxyDlg::OnBnClickedBtnTestoracle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	IDBConnect* pDBConn = wDBConnector::Create(_ORACLE_);
	if (NULL == pDBConn)
	{
		return;
	}

	const wchar *host   = _T("11.25.186.97/odsptdb");
	const wchar *user   = _T("ods");
	const wchar *passwd = _T("Ods97.ax");
	const wchar *db     = _T("axls_odsptdb");
	const int   port    = 1521;
	bool bRetValue = pDBConn->Connect(host, user, passwd, db, port);
	if (!bRetValue)
	{
		wDBConnector::Destroy(pDBConn);
		return;
	}

	IDBRecordSet* pDBRs = wDBConnector::Create(pDBConn);
	if (NULL == pDBRs)
	{
		wDBConnector::Destroy(pDBConn);
		return;
	}

	const wchar* sql = _T("select * from env_mon_item_evnet");
	bRetValue = pDBRs->Query(sql);
	if (!bRetValue)
	{
		goto END;
	}

	wchar* pValue = NULL;
	while(!pDBRs->IsEOF())
	{
		pValue = pDBRs->GetFieldValue(_T("item_name"));
		pDBRs->MoveNext();
	}

END:
	pDBRs->Close();
	pDBConn->Close();

	wDBConnector::Destroy(pDBRs);
	wDBConnector::Destroy(pDBConn);
}

void CdbsProxyDlg::OnBnClickedBtnTestmysql()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	IDBConnect* pDBConn = wDBConnector::Create(_MYSQL_);
	if (NULL == pDBConn)
	{
		return;
	}

	const wchar *host   = _T("11.25.186.222");
	const wchar *user   = _T("root");
	const wchar *passwd = _T("sunwe@123456");
	const wchar *db     = _T("emview5_core_db");
	const int   port    = 3306;
	bool bRetValue = pDBConn->Connect(host, user, passwd, db, port);
	if (!bRetValue)
	{
		wDBConnector::Destroy(pDBConn);
		return;
	}

	IDBRecordSet* pDBRs = wDBConnector::Create(pDBConn);
	if (NULL == pDBRs)
	{
		wDBConnector::Destroy(pDBConn);
		return;
	}

	const wchar* sql = _T("select * from ems_item_event");
	bRetValue = pDBRs->Query(sql);
	if (!bRetValue)
	{
		goto END;
	}

	wchar* pValue = NULL;
	while(!pDBRs->IsEOF())
	{
		pValue = pDBRs->GetFieldValue(_T("event_desc"));
		pDBRs->MoveNext();
	}

END:
	pDBRs->Close();
	pDBConn->Close();

	wDBConnector::Destroy(pDBRs);
	wDBConnector::Destroy(pDBConn);
}

void CdbsProxyDlg::OnBnClickedBtnEnvmon()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pEnvMonEvent = new CEnvMonEvent;
	m_pEnvMonEvent->SetMainHwnd(GetSafeHwnd());
	m_pEnvMonEvent->LoadParams();

	CPushManager::Instance().Init_Startup();
	CPushManager::Instance().AddPush(m_pEnvMonEvent);

	GetDlgItem(IDC_BTN_ENVMON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
}

void CdbsProxyDlg::OnBnClickedBtnStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPushManager::Instance().Stop_Release();

	if (NULL != m_pEnvMonEvent)
	{
		delete m_pEnvMonEvent;
		m_pEnvMonEvent = NULL;
	}

	GetDlgItem(IDC_BTN_ENVMON)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_EMS)->SetWindowText(_T("δ���У�������[0]"));
}

void CdbsProxyDlg::InitUI()
{
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);

	// mysql
	GetDlgItem(IDC_EDIT_MHOST)->SetWindowText(_T("11.25.186.222"));
	GetDlgItem(IDC_EDIT_MPORT)->SetWindowText(_T("3306"));
	GetDlgItem(IDC_EDIT_MDB)->SetWindowText(_T("emview5_core_db"));
	GetDlgItem(IDC_EDIT_MUSER)->SetWindowText(_T("root"));
	GetDlgItem(IDC_EDIT_MPASS)->SetWindowText(_T("sunwe@123456"));

	// oracle
	GetDlgItem(IDC_EDIT_OHOST)->SetWindowText(_T("11.25.186.97/odsptdb"));
	GetDlgItem(IDC_EDIT_OPORT)->SetWindowText(_T("1521"));
	GetDlgItem(IDC_EDIT_ODB)->SetWindowText(_T("axls_odsptdb"));
	GetDlgItem(IDC_EDIT_OUSER)->SetWindowText(_T("ods"));
	GetDlgItem(IDC_EDIT_OPASS)->SetWindowText(_T("Ods97.ax"));
}

void CdbsProxyDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (IDYES != MessageBox(_T("ȷ�Ϲرգ�"), _T("���ݴ���"), MB_ICONQUESTION | MB_YESNO))
	{
		return;
	}

	OnBnClickedBtnStop();
	CDialog::OnClose();
}
