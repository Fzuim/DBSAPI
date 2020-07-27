/** \file 
*	Log.h ������־ģ��
*
*	\author �ƶ�ǿ huangdq@tipray.com
*
*	�汾��ʷ
*	\par 2019-3-20
* 
*/

#pragma once

//////////////////////////////////////////////////////////////////////////
//����ArLog����־
#pragma comment(lib, "ArLog.lib")
extern void WINAPI DllAddMsgLog( const char *v_szMsg, const BOOL v_bTrace, const char* v_szFile );
extern void WINAPI DllAddLog( const char *v_strBuf, const int v_iBufLen, const BOOL v_bRecv, const char *v_szFile );
extern void WINAPI DllAddMsgLogW( const wchar_t* v_wszMsg, const wchar_t* v_wszFile, const int v_iLogType );
//////////////////////////////////////////////////////////////////////////
enum _enLogTypeW
{
	miALL   = 0,//��������־
	miDEBUG = 1,//ϸ������Ϣ�¼����Ե���Ӧ�ó����Ƿǳ��а�����   
	miINFO  = 2,//��������Ϣ�¼���ͻ��ǿ��Ӧ�ó�������й��� 
	miWARN  = 4,//���ܳ���Ǳ�ڴ��������   
	miERROR = 8,//��Ȼ���������¼�������Ȼ��Ӱ��ϵͳ�ļ�������
	miFATAL = 16,//ָ��ÿ�����صĴ����¼����ᵼ��Ӧ�ó�����˳�
};

//////////////////////////////////////////////////////////////////////////
#define MAX_BUF_LEN		1024
#define LOG_NAME		"dbsProxy.log"

class CLog  
{
private:
	CStringA m_cstrLogFileName;

public:
	//!< ��ӿ��ַ� miDEBUG������־
	void Debug(const wchar_t* lpFmt, ...)
	{
		wchar_t buff[MAX_BUF_LEN + 1] = {0};
		va_list arglist;
		va_start(arglist, lpFmt);
		_vsnwprintf_s(buff, MAX_BUF_LEN, lpFmt, arglist);
		va_end(arglist); 

		AddMsgLogW(buff, miDEBUG);
	}
	//!< ��ӿ��ַ� miINFO������־
	void Info(const wchar_t* lpFmt, ...)
	{
		wchar_t buff[MAX_BUF_LEN + 1] = {0};
		va_list arglist;
		va_start(arglist, lpFmt);
		_vsnwprintf_s(buff, MAX_BUF_LEN, lpFmt, arglist);
		va_end(arglist);

		AddMsgLogW(buff, miINFO);
	}
	//!< ��ӿ��ַ� miWARN������־
	void Warn(const wchar_t* lpFmt, ...)
	{
		wchar_t buff[MAX_BUF_LEN + 1] = {0};
		va_list arglist;
		va_start(arglist, lpFmt);
		_vsnwprintf_s(buff, MAX_BUF_LEN, lpFmt, arglist);
		va_end(arglist);

		AddMsgLogW(buff, miWARN);
	}
	//!< ��ӿ��ַ� miERROR������־
	void Error(const wchar_t* lpFmt, ...)
	{
		wchar_t buff[MAX_BUF_LEN + 1] = {0};
		va_list arglist;
		va_start(arglist, lpFmt);
		_vsnwprintf_s(buff, MAX_BUF_LEN, lpFmt, arglist);
		va_end(arglist);

		AddMsgLogW(buff, miERROR);
	}
	//!< ��ӿ��ַ� miFATAL������־
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