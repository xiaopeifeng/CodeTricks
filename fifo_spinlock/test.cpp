// spinlock.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Lock.h"
#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#include <vector>

#define SPIN_LOCK
int sum = 0;
#ifdef SPIN_LOCK
SpinLock g_lock;
#else
std::mutex g_lock;
#endif

void func(int i)
{
	g_lock.lock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	std::cout << std::this_thread::get_id() << "add : " << i << std::endl;
	sum++;
	g_lock.unlock();
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::thread t1(func, 1); 
	std::this_thread::sleep_for(std::chrono::microseconds(1));
	std::vector<std::thread*> vec;
	for (int i = 2; i < 50; i++)
	{
		std::thread* t = new std::thread(func, i);
		vec.push_back(t);
	}

	t1.join();
	for ( auto t : vec)
	{
		t->join();
	}

	std::cout << "finished, sum is : " << sum << std::endl;

	return 0;
}

