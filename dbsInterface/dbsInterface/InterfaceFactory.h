#pragma once
#include "BaseInterface.h"
#include "MySqlInterface.h"

class CInterfaceFactory
{
public:
	static CInterfaceFactory& Instance();
	CBaseInterface* CreateInterface(InterfaceType v_enInterfaceType);

private:
	CInterfaceFactory();
	~CInterfaceFactory();
};

