#pragma once
#include "IEventMonitor.h"
#include "IDBConnect.h"

class CEnvMonEvent : public IEventMonitor
{
public:
	CEnvMonEvent(void);
	~CEnvMonEvent(void);

	void LoadParams();
	void DoRefresh();

protected:
	bool CreateDBLink();
	bool SyncToOracle(IDBConnect* v_pOracleConnnector, wchar* v_pEventId, wchar* v_pItemName, wchar* v_pOccurTime, wchar* v_pEventDesc);
	void SaveParamsToIni();

private:
	IDBConnect* m_pMYSQLConnector;
	IDBConnect* m_pORACLEConnector;
	bool m_bCreateDBLickSuc;

	wchar m_tszLastOccurTime[PARAM_LEN];

	wchar m_tszMHost[PARAM_LEN];
	wchar m_tszMUser[PARAM_LEN];
	wchar m_tszMPass[PARAM_LEN];
	wchar m_tszMDB[PARAM_LEN];
	int	  m_iMPort;

	wchar m_tszOHost[PARAM_LEN];
	wchar m_tszOUser[PARAM_LEN];
	wchar m_tszOPass[PARAM_LEN];
	wchar m_tszODB[PARAM_LEN];
	int	  m_iOPort;
};
