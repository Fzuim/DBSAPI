#pragma once

class CPubFunc
{
public:
	CPubFunc();
	~CPubFunc();

	static void GetModulePath(HMODULE v_hModule, TCHAR* v_tszModPath);
};

