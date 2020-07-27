#include "stdafx.h"
#include "PubFunc.h"


CPubFunc::CPubFunc()
{
}


CPubFunc::~CPubFunc()
{
}

void CPubFunc::GetModulePath(HMODULE v_hModule, TCHAR* v_tszModPath)
{
	/// 获取当前工作目录
	GetModuleFileName(v_hModule, v_tszModPath, MAX_PATH);
	int iLen = _tcslen(v_tszModPath);

	for (int i = iLen - 1; i >= 0; i--)
	{
		if (_T('\\') == v_tszModPath[i])
		{
			v_tszModPath[i] = _T('\0');
			break;
		}
	}
}
