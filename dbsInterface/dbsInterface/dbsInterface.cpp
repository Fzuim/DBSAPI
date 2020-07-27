// dbsInterface.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "dbsInterface.h"
#include "InterfaceFactory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CdbsInterfaceApp

BEGIN_MESSAGE_MAP(CdbsInterfaceApp, CWinApp)
END_MESSAGE_MAP()


// CdbsInterfaceApp 构造

CdbsInterfaceApp::CdbsInterfaceApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CdbsInterfaceApp 对象

CdbsInterfaceApp theApp;


// CdbsInterfaceApp 初始化

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