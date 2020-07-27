#pragma once

class CPubFunc
{
public:
	static void GetCurExePath(TCHAR* v_tszExePath)
	{
		/// 获取当前工作目录
		GetModuleFileName(NULL, v_tszExePath, MAX_PATH);
		int iLen = _tcslen( v_tszExePath );

		for(int i = iLen - 1; i >= 0; i--)
		{
			if(_T('\\') == v_tszExePath[i])
			{
				v_tszExePath[i] = _T('\0');
				break;
			}
		}
	}
};