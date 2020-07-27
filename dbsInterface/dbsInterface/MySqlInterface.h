#pragma once
#include "BaseInterface.h"

class CMySqlInterface : public CBaseInterface
{
public:
	CMySqlInterface();
	~CMySqlInterface();

	int Init_Startup();
	int Stop_Release();

	MYSQL_RES* QueryData(const CStringA& v_cstrSqlA, MYSQL_RES** v_pResult);
	DWORD ExecuteSQL(const CStringA& v_cstrSqlA);
	void DeleteRES(MYSQL_RES** v_pResult);

protected:
	BOOL IsConnected();

private:
	MYSQL m_MySqlData;

	CMyCriticalSectionLock m_Lock;
};

