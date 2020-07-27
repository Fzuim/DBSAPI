#include "stdafx.h"
#include "BaseInterface.h"


CBaseInterface::CBaseInterface()
{
	memset(&m_ConnectInfo, 0, sizeof(CONNECTINFO));
}

CBaseInterface::~CBaseInterface()
{
}

int CBaseInterface::Init_Startup()
{
	return 0;
}

int CBaseInterface::Stop_Release()
{
	return 0;
}

void CBaseInterface::SetConnectInfo(const CONNECTINFO& v_ConnectInfo)
{
	memcpy(&m_ConnectInfo, &v_ConnectInfo, sizeof(CONNECTINFO));
}
