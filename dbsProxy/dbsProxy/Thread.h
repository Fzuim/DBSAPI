/** \file 
*	Thread.h，定义线程创建和停止接口，通过内联函数提供给其它模块使用
*
*	\author Fzuim
*
*	版本历史
*	\par 2019-3-20
*
*/

#pragma once

#include <process.h>

typedef unsigned int (WINAPI *THREAD_FUNC)(LPVOID lpThreadParameter);

class CThread
{
public:
	CThread(THREAD_FUNC ThreadFunc,PVOID ThreadArg, int priority = THREAD_PRIORITY_NORMAL)
	{
		m_threadFunc = ThreadFunc;
		m_threadArg = ThreadArg;

		m_hThead = (HANDLE)_beginthreadex(NULL, 
			0,
			(THREAD_FUNC)m_threadFunc,
			ThreadArg,
			CREATE_SUSPENDED,
			(unsigned int *)&m_dwThreadID);

		if(INVALID_HANDLE_VALUE != m_hThead)
		{
			SetThreadPriority(m_hThead, priority);
		}
	}

	~CThread()
	{
		if(m_hThead && INVALID_HANDLE_VALUE != m_hThead)
		{
			::CloseHandle(m_hThead);    
		}
	}

	void Run()
	{
		if(m_hThead && INVALID_HANDLE_VALUE != m_hThead)
		{
			::ResumeThread(m_hThead);
		}
	}

	void Stop()
	{
		if(NULL != m_hThead && INVALID_HANDLE_VALUE != m_hThead)
		{
			TerminateThread(m_hThead, 0); //!<设置线程退出码为0

			CloseHandle(m_hThead);
			m_hThead = NULL;
		}
	}

	DWORD GetExitCode()
	{
		DWORD dwRet = 0;

		::GetExitCodeThread(m_hThead,&dwRet);

		return dwRet;
	}

	HANDLE      m_hThead;
	DWORD	    m_dwThreadID;
private:
	THREAD_FUNC m_threadFunc;
	PVOID	    m_threadArg;
};

#define RELEASE_CTHREAD_RES(CTHREAD_PTR)										\
	if (NULL != CTHREAD_PTR)													\
{																			\
	if (WAIT_OBJECT_0 != WaitForSingleObject(CTHREAD_PTR->m_hThead, 100L))	\
{																		\
	CTHREAD_PTR->Stop();												\
}																		\
	delete CTHREAD_PTR;														\
	CTHREAD_PTR = NULL;														\
}