#include "stdafx.h"
#include "MySqlInterface.h"

CMySqlInterface::CMySqlInterface()
{
	SetInitResult(enUnInit);
}

CMySqlInterface::~CMySqlInterface()
{
}

int CMySqlInterface::Init_Startup()
{
	int iRet = -1;

	CONNECTINFO connectInfo = GetConnectInfo();

	do 
	{
		if (enInited == GetInitResult())
		{ // 已初始化，直接返回
			iRet = 0;
			break;
		}

		if (0 != mysql_library_init(0, NULL, NULL))
		{
			iRet = mysql_errno(&m_MySqlData);
			break;
		}

		if (NULL == mysql_init(&m_MySqlData))
		{
			iRet = mysql_errno(&m_MySqlData);
			break;
		}

		if (0 != mysql_options(&m_MySqlData, MYSQL_SET_CHARSET_NAME, "utf8"))
		{
			iRet = mysql_errno(&m_MySqlData);
			break;
		}

		// 连接数据库，并登录
		if (NULL == mysql_real_connect(&m_MySqlData, connectInfo.m_szHost, connectInfo.m_szLoginName, 
			connectInfo.m_szLoginPass, connectInfo.m_szDBName, connectInfo.m_dwPort, NULL, 0))
		{
			iRet = mysql_errno(&m_MySqlData);
			break;
		}

		iRet = 1;
		// 初始化完成
		SetInitResult(enInited);
	} while (FALSE);
	
	return iRet;
}

int CMySqlInterface::Stop_Release()
{
	mysql_close(&m_MySqlData);

	return 1;
}

MYSQL_RES* CMySqlInterface::QueryData(const CStringA& v_cstrSqlA, MYSQL_RES** v_pResult)
{
	AUTO_LOCK(m_Lock);

	try
	{
		if (0 == mysql_query(&m_MySqlData, v_cstrSqlA))
		{
			*v_pResult = mysql_store_result(&m_MySqlData);
		}
	}
	catch (...)
	{
		CLogModule::Odprintf(_T("~!@ %s Err"), __FUNCTIONW__);
	}

	return *v_pResult;
}

DWORD CMySqlInterface::ExecuteSQL(const CStringA& v_cstrSqlA)
{
	AUTO_LOCK(m_Lock);

	DWORD dwCount = 0;

	if (0 == mysql_query(&m_MySqlData, v_cstrSqlA))
	{
		MYSQL_RES* pResult = NULL;
		dwCount = (DWORD)mysql_affected_rows(&m_MySqlData);
		pResult = mysql_store_result(&m_MySqlData);
		if (NULL != pResult)
		{
			mysql_free_result(pResult);
			pResult = NULL;
		}
	}

	return dwCount;
}

void CMySqlInterface::DeleteRES(MYSQL_RES** v_pResult)
{
	try
	{
		if (NULL != *v_pResult)
		{
			mysql_free_result(*v_pResult);
			*v_pResult = NULL;
		}
	}
	catch (...)
	{
		CLogModule::Odprintf(_T("~!@ %s Err"), __FUNCTIONW__);
		*v_pResult = NULL;
	}
}

BOOL CMySqlInterface::IsConnected()
{
	AUTO_LOCK(m_Lock);

	BOOL bRet = TRUE;

	if (0 != mysql_ping(&m_MySqlData))
	{
		bRet = FALSE;
	}

	return bRet;
}
