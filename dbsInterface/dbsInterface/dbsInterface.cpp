// dbsInterface.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "dbsInterface.h"
#include "InterfaceFactory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CdbsInterfaceApp

BEGIN_MESSAGE_MAP(CdbsInterfaceApp, CWinApp)
END_MESSAGE_MAP()


// CdbsInterfaceApp ����

CdbsInterfaceApp::CdbsInterfaceApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CdbsInterfaceApp ����

CdbsInterfaceApp theApp;


// CdbsInterfaceApp ��ʼ��

BOOL CdbsInterfaceApp::InitInstance()
{
	CWinApp::InitInstance();

	CPubFunc::GetModulePath(NULL, g_Invoke_pi.m_tszCurExePath);
	CLogModule::Odprintf(_T("~!@ %s CurExePath[%s]"), __FUNCTIONW__, g_Invoke_pi.m_tszCurExePath);

	return TRUE;
}

void WINAPI DLLTest(BYTE v_byDBType)
{
	CBaseInterface* pDBInterface = CInterfaceFactory::Instance().CreateInterface((InterfaceType)v_byDBType);
	if (NULL != pDBInterface)
	{
		CONNECTINFO connectInfo;
		memset(&connectInfo, 0, sizeof(CONNECTINFO));

		strcpy_s(connectInfo.m_szHost, "11.25.186.222");
		strcpy_s(connectInfo.m_szLoginName, "root");
		strcpy_s(connectInfo.m_szLoginPass, "sunwe@123456");
		strcpy_s(connectInfo.m_szDBName, "emview5_data_db");
		connectInfo.m_dwPort = 3306;

		pDBInterface->SetConnectInfo(connectInfo);
		pDBInterface->Init_Startup();

		MYSQL_RES* pResult = NULL;
		MYSQL_RES* pTmpRes = NULL;
		MYSQL_ROW row = NULL;
		pResult = ((CMySqlInterface*)pDBInterface)->QueryData("select * from log_itemgroup_event", &pTmpRes);
		if (NULL != pResult)
		{
			row = mysql_fetch_row(pResult);
			while (NULL != row)
			{
				if (NULL != row[0])
				{
					CLogModule::Odprintf("~!@ row[0]=%s", row[0]);
				}

				if (NULL != row[1])
				{
					CLogModule::Odprintf("~!@ row[1]=%s", row[1]);
				}

				row = mysql_fetch_row(pResult);
			}

			((CMySqlInterface*)pDBInterface)->DeleteRES(&pTmpRes);
		}
	}
}