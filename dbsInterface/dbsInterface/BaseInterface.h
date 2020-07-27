#pragma once

typedef struct _tagConnectInfo
{
	char m_szHost[MAX_PATH];
	DWORD m_dwPort;
	char m_szLoginName[MAX_PATH];
	char m_szLoginPass[MAX_PATH];
	char m_szDBName[MAX_PATH];
}CONNECTINFO;

// 接口基类
class CBaseInterface
{
public:
	CBaseInterface();
	~CBaseInterface();

	virtual int Init_Startup();
	virtual int Stop_Release();

	void SetConnectInfo(const CONNECTINFO& v_ConnectInfo);
	CONNECTINFO& GetConnectInfo(){ return m_ConnectInfo; };

	void SetInitResult(InitResult v_iInitResult){ m_iInitResult = v_iInitResult; };
	InitResult GetInitResult(){ return m_iInitResult; };

private:
	InitResult m_iInitResult;
	CONNECTINFO m_ConnectInfo;
};

