#pragma once
#include "IEventMonitor.h"

#ifdef _DEBUG
const int SLEEPTIME = 3000;
#else
const int SLEEPTIME = 60 * 1000;
#endif

class CEventMonNode
{
public:
	CEventMonNode(){};
	~CEventMonNode(){};

	IEventMonitor* m_pEventMon;
};

class CEventMonFunList
{
public:
	CEventMonFunList(){};
	~CEventMonFunList()
	{
		AUTO_LOCK(m_Lock);
		while(!m_FunList.IsEmpty())
		{
			CEventMonNode* pNode = (CEventMonNode*)m_FunList.RemoveHead();
			if (NULL != pNode)
			{
				delete pNode;
				pNode = NULL;
			}
		}
	};

	void AddFun(IEventMonitor* v_pMonFun)
	{
		AUTO_LOCK(m_Lock);
		CEventMonNode* pNode = new CEventMonNode;
		if (NULL != pNode)
		{
			pNode->m_pEventMon = v_pMonFun;

			m_FunList.AddTail((CObject*)pNode);
		}
	}

	void Excute()
	{
		AUTO_LOCK(m_Lock);
		for (POSITION pos = m_FunList.GetHeadPosition(); pos != NULL;)
		{
			CEventMonNode* pNode = (CEventMonNode*)m_FunList.GetNext(pos);
			if (NULL != pNode && NULL != pNode->m_pEventMon)
			{
				pNode->m_pEventMon->DoRefresh();
			}
		}
	}
private:
	CObList m_FunList;
	// ¼ÓËø
	CMyCriticalSectionLock m_Lock;
};

class CPushManager
{
public:
	static CPushManager& Instance();
	void Init_Startup();
	void Stop_Release();

	void AddPush(IEventMonitor* v_pEventMon);

protected:
	static UINT WINAPI s_PushProc(LPVOID v_lpParam);
	void PushTimer();
	void doPush();

private:
	CPushManager(void);
	~CPushManager(void);

	CEventMonFunList* m_pEventMonList;
	CThread* m_pPushThd;
	BOOL m_bStopPushThd;
};
