/** \file 
*	Log.h 定义日志模块
*
*	\author 黄东强 huangdq@tipray.com
*
*	版本历史
*	\par 2019-3-20
* 
*/

#pragma once

//////////////////////////////////////////////////////////////////////////
//采用ArLog打日志
#pragma comment(lib, "ArLog.lib")
extern void WINAPI DllAddMsgLog( const char *v_szMsg, const BOOL v_bTrace, const char* v_szFile );
extern void WINAPI DllAddLog( const char *v_strBuf, const int v_iBufLen, const BOOL v_bRecv, const char *v_szFile );
extern void WINAPI DllAddMsgLogW( const wchar_t* v_wszMsg, const wchar_t* v_wszFile, const int v_iLogType );
//////////////////////////////////////////////////////////////////////////
enum _enLogTypeW
{
	miALL   = 0,//打开所有日志
	miDEBUG = 1,//细粒度信息事件，对调试应用程序是非常有帮助的   
	miINFO  = 2,//粗粒度信息事件，突出强调应用程序的运行过程 
	miWARN  = 4,//可能出现潜在错误的情形   
	miERROR = 8,//虽然发生错误事件，但仍然不影响系统的继续运行
	miFATAL = 16,//指出每个严重的错误事件将会导致应用程序的退出
};

//////////////////////////////////////////////////////////////////////////
#define MAX_BUF_LEN		1024
#define LOG_NAME		"dbsProxy.log"

class CLog  
{
private:
	CStringA m_cstrLogFileName;

public:
	//!< 添加宽字符 miDEBUG类型日志
	void Debug(const wchar_t* lpFmt, ...)
	{
		wchar_t buff[MAX_BUF_LEN + 1] = {0};
		va_list arglist;
		va_start(arglist, lpFmt);
		_vsnwprintf_s(buff, MAX_BUF_LEN, lpFmt, arglist);
		va_end(arglist); 

		AddMsgLogW(buff, miDEBUG);
	}
	//!< 添加宽字符 miINFO类型日志
	void Info(const wchar_t* lpFmt, ...)
	{
		wchar_t buff[MAX_BUF_LEN + 1] = {0};
		va_list arglist;
		va_start(arglist, lpFmt);
		_vsnwprintf_s(buff, MAX_BUF_LEN, lpFmt, arglist);
		va_end(arglist);

		AddMsgLogW(buff, miINFO);
	}
	//!< 添加宽字符 miWARN类型日志
	void Warn(const wchar_t* lpFmt, ...)
	{
		wchar_t buff[MAX_BUF_LEN + 1] = {0};
		va_list arglist;
		va_start(arglist, lpFmt);
		_vsnwprintf_s(buff, MAX_BUF_LEN, lpFmt, arglist);
		va_end(arglist);

		AddMsgLogW(buff, miWARN);
	}
	//!< 添加宽字符 miERROR类型日志
	void Error(const wchar_t* lpFmt, ...)
	{
		wchar_t buff[MAX_BUF_LEN + 1] = {0};
		va_list arglist;
		va_start(arglist, lpFmt);
		_vsnwprintf_s(buff, MAX_BUF_LEN, lpFmt, arglist);
		va_end(arglist);

		AddMsgLogW(buff, miERROR);
	}
	//!< 添加宽字符 miFATAL类型日志
	void Fatal(const wchar_t* lpFmt, ...)
	{
		wchar_t buff[MAX_BUF_LEN + 1] = {0};
		va_list arglist;
		va_start(arglist, lpFmt);
		_vsnwprintf_s(buff, MAX_BUF_LEN, lpFmt, arglist);
		va_end(arglist);

		AddMsgLogW(buff, miFATAL);
	}

	void AddMsgLogW( const WCHAR* v_wszMsg, const int v_iLogType=miALL )
	{
		static      DWORD dwPId = 0;
		if (0 == dwPId)
		{
			dwPId = GetCurrentProcessId();
		}
		wchar_t wszMsg[MAX_BUF_LEN + 10]={0};
		wchar_t wszLogFileName[MAX_PATH*4]={0};

		_snwprintf_s(wszMsg, MAX_BUF_LEN, L"[%d]%ws",dwPId,v_wszMsg);

		if ( "" != m_cstrLogFileName)
		{
			swprintf(wszLogFileName,L"%ws",m_cstrLogFileName.AllocSysString());

			DllAddMsgLogW( wszMsg, wszLogFileName, v_iLogType );
		}	
	};

	void Init( const char* v_szLogFileName )
	{
		m_cstrLogFileName = v_szLogFileName;
	};


	CLog(){};

	virtual ~CLog(){};
};