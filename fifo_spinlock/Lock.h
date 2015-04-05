#pragma once
#include <windows.h>
//功能：实现FIFIO特征的的自旋锁
//实现思路：
//在客户线程添加一个标记，当现场试图去lock时，如果没有占用成功，标记自己的ticket,即抢占的次序
//自旋锁的数据结构里面会记录下一个可以使用的线程，也就是说数据结构里面保留了下一个抢占线程的ticket
//只有这个线程来抢占时才会成功

//code by fxp


class SpinLock
{
public:
	SpinLock()
	{
		m_status = 0;
		m_cur_ticket = 0;
		m_next_ticket = 0;
		m_tickets = 0;
	}

	~SpinLock()
	{}

public:

	bool try_lock(long ticket)
	{
		if (ticket != m_next_ticket) return false;
		long l = InterlockedExchangeAdd((long*)(&m_status), 1);
		return l == 0;
	}

	void lock()
	{
		long old_ticket = InterlockedExchangeAdd((long*)(&m_tickets), 1);
		for (unsigned u = 0; !try_lock(old_ticket); ++u);
	}

	void unlock()
	{
		long old_ticket = InterlockedExchangeAdd((long*)(&m_next_ticket), 1);
		m_status = 0;
	}

private:
	volatile long m_status;
	volatile long m_cur_ticket;
	volatile long m_next_ticket;
	volatile long m_tickets;
};
