#pragma once

// mysql安装32或64，项目就必须为32或64
#include <mysql.h>

#include "LogModule.h"
#include "PubFunc.h"
#include "AutoLock.h"
#include "Ado.h"

enum InterfaceType
{
	enMySql = 1,
	enOracle = 2,
	enSqlite = 3
};

enum InitResult
{
	enUnInit = 0,
	enInited = 1,
	enInitErr1 = 2,
};

#define MAX_PATH_EXP 2 * MAX_PATH

typedef struct _tagInvokeProcessInfo
{
	TCHAR m_tszCurExePath[MAX_PATH_EXP];
}INVOKE_PROCESS_INFO;

extern INVOKE_PROCESS_INFO g_Invoke_pi;