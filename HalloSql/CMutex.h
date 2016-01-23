

#ifndef __MUTEX_H__
#define __MUTEX_H__

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <time.h>
#include <errno.h>

// 是否计算锁定时间
// #define MUTEX_LOCK_TIME

// 互斥体
class CMutex
{
public:

	CMutex()
	{
		m_wait_time = 0;
#ifdef _WIN32
		InitializeCriticalSection(&m_Section);
#else
		pthread_mutex_init(&mutex_, NULL);
#endif
	}

	~CMutex()
	{
#ifdef _WIN32
		DeleteCriticalSection(&m_Section);
#else
		pthread_mutex_destroy(&mutex_);
#endif
	}

	int lock()
	{

#ifdef MUTEX_LOCK_TIME
		clock_t start = clock();
#endif


#ifdef _WIN32
		EnterCriticalSection(&m_Section);
#else
		pthread_mutex_lock(&mutex_);
#endif


#ifdef MUTEX_LOCK_TIME
		int wait_time = (int)(clock() - start) * 1000 / CLOCKS_PER_SEC;
		m_wait_time += wait_time;
		return	wait_time;
#else
		return	0;
#endif
	}

	void unlock()
	{
#ifdef _WIN32
		LeaveCriticalSection(&m_Section);
#else
		pthread_mutex_unlock(&mutex_);
#endif
	}


#ifdef _WIN32
#if(_WIN32_WINNT >= 0x0400)
	bool trylock()
	{
		return TryEnterCriticalSection(&m_Section)!=0;
	}
#endif
#else
	bool trylock()
	{
		return (EBUSY != pthread_mutex_trylock(&mutex_));
	}
#endif

private:
#ifdef _WIN32
	CRITICAL_SECTION		m_Section;
#else
	pthread_mutex_t			mutex_;
#endif

public:
	int						m_wait_time; // 等待时间
};


// 互斥体守卫
class CMutexGuard
{
public:
	CMutexGuard(CMutex& m)
	{
		m_pMutex = &m;
		m_pMutex->lock();
	}

	~CMutexGuard()
	{
		if (m_pMutex)
		{
			m_pMutex->unlock();
		}
	}

private:
	CMutex*		m_pMutex;
};

#if (_WIN32_WINNT >= 0x0400) || !defined(_WIN32)
class CMutexGuard_TryLock
{
public:
	CMutexGuard_TryLock(CMutex& m)
	{
		m_pMutex = &m;
		m_bLock = m_pMutex->trylock();
	}

	operator bool() { return m_bLock; }

	~CMutexGuard_TryLock()
	{
		if (m_pMutex && m_bLock)
		{
			m_pMutex->unlock();
		}
	}

private:
	bool		m_bLock;
	CMutex*		m_pMutex;
};
#endif

#endif // __MUTEX_H__

