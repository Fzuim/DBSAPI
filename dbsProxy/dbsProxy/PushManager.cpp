#include "StdAfx.h"
#include "PushManager.h"

CPushManager::CPushManager(void)
{
	m_bStopPushThd = FALSE;
	m_pPushThd = NULL;
	m_pEventMonList = NULL;
}

CPushManager::~CPushManager(void)
{
}

CPushManager& CPushManager::Instance()
{
	static CPushManager instance;
	return instance;
}

void CPushManager::Init_Startup()
{
	m_bStopPushThd = FALSE;
	m_pEventMonList = new CEventMonFunList;
	m_pPushThd = new CThread(CPushManager::s_PushProc, this);
	if (NULL != m_pPushThd)
	{
		m_pPushThd->Run();
		CLogModule::Odprintf(_T("~!@ [%s]Create Push Thread[%d]"), _FUNCTION_, m_pPushThd->m_dwThreadID);
	}
}

void CPushManager::Stop_Release()
{
	m_bStopPushThd = TRUE;

	if (NULL != m_pPushThd)
	{
		if (WAIT_OBJECT_0 != WaitForSingleObject(m_pPushThd->m_hThead, 1000L))
		{
			m_pPushThd->Stop();
		}

		delete m_pPushThd;
		m_pPushThd = NULL;
	}

	if (NULL != m_pEventMonList)
	{
		delete m_pEventMonList;
		m_pEventMonList = NULL;
	}
}

UINT WINAPI CPushManager::s_PushProc(LPVOID v_lpParam)
{
	CPushManager* pThis = (CPushManager*)v_lpParam;
	pThis->PushTimer();

	return 0;
}

void CPushManager::PushTimer()
{
	while(!m_bStopPushThd)
	{
		doPush();
		Sleep(SLEEPTIME);
	}
}

void CPushManager::doPush()
{
	//CLogModule::Odprintf(_T("~!@ [%s] start..."), _FUNCTION_);
	m_pEventMonList->Excute();
	//CLogModule::Odprintf(_T("~!@ [%s] end..."), _FUNCTION_);
}

void CPushManager::AddPush(IEventMonitor* v_pEventMon)
{
	m_pEventMonList->AddFun(v_pEventMon);
	CLogModule::Odprintf(_T("~!@ [%s]Func[%p]"), _FUNCTION_, v_pEventMon);
}