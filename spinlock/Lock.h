#pragma once
#include <windows.h>

class SpinLock
{
public:
	SpinLock()
	{
		m_status = 0;
	}

	~SpinLock()
	{}

public:

	bool try_lock()
	{
		long l = InterlockedExchangeAdd((long*)(&m_status), 1);
		return l == 0;
	}

	void lock()
	{
		for (unsigned u = 0; !try_lock(); ++u);
	}

	void unlock()
	{
		m_status = 0;
	}

private:
	volatile long m_status;
};
