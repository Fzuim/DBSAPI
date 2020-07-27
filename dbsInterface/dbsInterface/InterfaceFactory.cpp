#include "stdafx.h"
#include "InterfaceFactory.h"


CInterfaceFactory::CInterfaceFactory()
{
}


CInterfaceFactory::~CInterfaceFactory()
{
}

CInterfaceFactory& CInterfaceFactory::Instance()
{
	static CInterfaceFactory singleton;
	return singleton;
}

CBaseInterface* CInterfaceFactory::CreateInterface(InterfaceType v_enInterfaceType)
{
	CBaseInterface* pInterface = NULL;

	switch (v_enInterfaceType)
	{
	case enMySql:
		pInterface = new CMySqlInterface;
		break;
	case enOracle:
		break;
	case enSqlite:
		break;
	default:
		break;
	}

	return pInterface;
}
