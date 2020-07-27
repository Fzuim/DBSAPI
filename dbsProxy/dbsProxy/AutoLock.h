/** \AutoLock.h
*	该文件中，实现自动锁
*
*	\author 黄东强 haugndq@tipray.com
*
*	版本历史
*	\par 2019-9-20
*	
*/

#pragma once

class IMyLock
{
public:
	IMyLock(){};
	virtual ~IMyLock(){};

	virtual void Lock() = 0;
	virtual void UnLock() = 0;
};

class CMyCriticalSectionLock : public IMyLock
{
public:
	CMyCriticalSectionLock(){
		InitializeCriticalSection(&m_cs);
	};

	~CMyCriticalSectionLock(){
		DeleteCriticalSection(&m_cs);
	}

	virtual void Lock()
	{
		EnterCriticalSection(&m_cs);
	}

	virtual void UnLock()
	{
		LeaveCriticalSection(&m_cs);
	}

private:
	CRITICAL_SECTION m_cs;
};

class CMyMutexLock : public IMyLock
{
public:
	CMyMutexLock(){
		m_mutex = ::CreateMutex(NULL, FALSE, NULL);
	}

	CMyMutexLock(const TCHAR* v_tszName){
		m_mutex = ::CreateMutex(NULL, FALSE, v_tszName);
	}

	~CMyMutexLock(){
		::CloseHandle(m_mutex);
	}

	virtual void Lock()
	{
		DWORD d = WaitForSingleObject(m_mutex, INFINITE);
	}

	virtual void UnLock()
	{
		::ReleaseMutex(m_mutex);
	}

private:
	HANDLE m_mutex;
};

class CAutoLock
{
public:
	CAutoLock(IMyLock& v_Lock) : m_Lock(v_Lock)
	{
		m_Lock.Lock();
	}

	~CAutoLock(){
		m_Lock.UnLock();
	}

private:
	IMyLock& m_Lock;
};

#define AUTO_LOCK(LOCK) \
	CAutoLock _autolock(LOCK)