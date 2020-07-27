#include "StdAfx.h"
#include "EnvMonEvent.h"

CEnvMonEvent::CEnvMonEvent(void)
{

}

CEnvMonEvent::~CEnvMonEvent(void)
{
}

void CEnvMonEvent::LoadParams()
{
	TCHAR tszCurExePath[2 * MAX_PATH] = {0};
	TCHAR tszIniPath[2 * MAX_PATH] = {0};
	CPubFunc::GetCurExePath(tszCurExePath);
	_stprintf_s(tszIniPath, _T("%s\\%s"), tszCurExePath, CONFIG_INI);

	GetPrivateProfileString(_T("config"), _T("lasttime"), _T("2020-01-01 00:00:00"), m_tszLastOccurTime, sizeof(m_tszLastOccurTime), tszIniPath);

	::GetDlgItemText(GetMainHwnd(), IDC_EDIT_MHOST, m_tszMHost, sizeof(m_tszMHost));
	::GetDlgItemText(GetMainHwnd(), IDC_EDIT_MDB, m_tszMDB, sizeof(m_tszMDB));
	::GetDlgItemText(GetMainHwnd(), IDC_EDIT_MUSER, m_tszMUser, sizeof(m_tszMUser));
	::GetDlgItemText(GetMainHwnd(), IDC_EDIT_MPASS, m_tszMPass, sizeof(m_tszMPass));

	TCHAR tszValue[PARAM_LEN] = {0};
	::GetDlgItemText(GetMainHwnd(), IDC_EDIT_MPORT, tszValue, sizeof(tszValue));
	m_iMPort = _ttoi(tszValue);

	::GetDlgItemText(GetMainHwnd(), IDC_EDIT_OHOST, m_tszOHost, sizeof(m_tszOHost));
	::GetDlgItemText(GetMainHwnd(), IDC_EDIT_ODB, m_tszODB, sizeof(m_tszODB));
	::GetDlgItemText(GetMainHwnd(), IDC_EDIT_OUSER, m_tszOUser, sizeof(m_tszOUser));
	::GetDlgItemText(GetMainHwnd(), IDC_EDIT_OPASS, m_tszOPass, sizeof(m_tszOPass));

	::GetDlgItemText(GetMainHwnd(), IDC_EDIT_OPORT, tszValue, sizeof(tszValue));
	m_iOPort = _ttoi(tszValue);
}

void CEnvMonEvent::DoRefresh()
{
	IDBConnect*		pMySqlConnector		= NULL;
	IDBConnect*		pOracleConnector	= NULL;
	IDBRecordSet*	pDBRs				= NULL;
	wchar*			pEventId				= NULL;
	wchar*			pItemName			= NULL;
	wchar*			pOccurTime			= NULL;
	wchar*			pEventDesc			= NULL;

	int iErrCode = 0;

	//CLogModule::Odprintf(_T("~!@ [%s] start..."), _FUNCTION_);

	do 
	{
		pMySqlConnector = wDBConnector::Create(_MYSQL_);
		if (NULL == pMySqlConnector)
		{
			iErrCode = 1000;
			break;
		}

		if (!pMySqlConnector->Connect(m_tszMHost, m_tszMUser, m_tszMPass, m_tszMDB, m_iMPort))
		{
			iErrCode = 1001;
			wDBConnector::Destroy(pMySqlConnector);
			pMySqlConnector = NULL;
			break;
		}

		pDBRs = wDBConnector::Create(pMySqlConnector);
		if (NULL == pDBRs)
		{
			iErrCode = 1002;
			wDBConnector::Destroy(pMySqlConnector);
			pMySqlConnector = NULL;
			break;
		}

		wchar tszQuerySQL[SQL_LEN] = {0};
		_stprintf_s(tszQuerySQL, _T("select id,item_name,occur_time,event_desc from ems_item_event where occur_time > '%s' order by occur_time desc"), 
			m_tszLastOccurTime);
		
		//theApp.m_Log.Debug(_T("[%s]QuerySQL[%s]"), _FUNCTION_, tszQuerySQL);

		if (pDBRs->Query(tszQuerySQL) && !pDBRs->IsEOF())
		{
			pOracleConnector = wDBConnector::Create(_ORACLE_);
			if (NULL != pOracleConnector && pOracleConnector->Connect(m_tszOHost, m_tszOUser, m_tszOPass, m_tszODB, m_iOPort))
			{
				pOccurTime = pDBRs->GetFieldValue(_T("occur_time"));
				if (NULL != pOccurTime)
				{
					_tcscpy_s(m_tszLastOccurTime, pOccurTime);
					SaveParamsToIni();
					CLogModule::Odprintf(_T("~!@ [%s]Lsat Event Occur Time[%s]"), _FUNCTION_, m_tszLastOccurTime);
				}

				while(!pDBRs->IsEOF())
				{
					pEventId = pDBRs->GetFieldValue(_T("id"));
					pItemName = pDBRs->GetFieldValue(_T("item_name"));
					pOccurTime = pDBRs->GetFieldValue(_T("occur_time"));
					pEventDesc = pDBRs->GetFieldValue(_T("event_desc"));

					if (!SyncToOracle(pOracleConnector, pEventId, pItemName, pOccurTime, pEventDesc))
					{
						iErrCode = 1003;
					}

					pDBRs->MoveNext();
				}

				pOracleConnector->Close();
				wDBConnector::Destroy(pOracleConnector);
			}
		}

		pDBRs->Close();
		pMySqlConnector->Close();
		wDBConnector::Destroy(pDBRs);
		wDBConnector::Destroy(pMySqlConnector);

	} while (FALSE);

	if (NULL != GetMainHwnd())
	{
		PREFRESHMSG pMsg = new REFRESHMSG;
		memset(pMsg, 0, sizeof(REFRESHMSG));

		pMsg->m_iMsgType = _ENVMSG_;
		pMsg->m_iMsgFlag = 1;
		_stprintf_s(pMsg->m_tszMsg, _T("运行中，错误信息[%d]，最近发生事件时间[%s]"), iErrCode, m_tszLastOccurTime);

		::SendMessage(GetMainHwnd(), WM_REFRESHMSG, (WPARAM)pMsg, NULL);
	}
}

bool CEnvMonEvent::SyncToOracle(IDBConnect* v_pOracleConnnector, wchar* v_pEventId, wchar* v_pItemName, wchar* v_pOccurTime, wchar* v_pEventDesc)
{
	std::string strEventDesc(v_pEventDesc);
	Fzuim::StringUtility::string_replace("%", "-", &strEventDesc);

	wchar tszInsertSQL[SQL_LEN * 2] = {0};
	_stprintf_s(tszInsertSQL, _T("insert into env_mon_item_evnet (id,item_name,occur_time,event_desc,status) values ('%s','%s','%s','%s',0)"),
		v_pEventId, v_pItemName, v_pOccurTime, strEventDesc.c_str());

	bool bRet = v_pOracleConnnector->Execute(tszInsertSQL);

	return bRet;
}

void CEnvMonEvent::SaveParamsToIni()
{
	TCHAR tszCurExePath[2 * MAX_PATH] = {0};
	TCHAR tszIniPath[2 * MAX_PATH] = {0};
	CPubFunc::GetCurExePath(tszCurExePath);
	_stprintf_s(tszIniPath, _T("%s\\%s"), tszCurExePath, CONFIG_INI);

	TCHAR tszValue[PARAM_LEN] = {0};
	WritePrivateProfileString(_T("config"), _T("lasttime"), m_tszLastOccurTime, tszIniPath);
}

bool CEnvMonEvent::CreateDBLink()
{
	return false;
}

