// memory_pool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "pool.h"
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtx;

class obj
{
public:
	explicit obj(const std::string& str1, const std::string& str2)
		: m_name(str1)
		, m_address(str2)
	{
		std::cout << "obj constructor called." << std::endl;
	}

	void print()
	{
		std::cout << "name:" << m_name << std::endl;
		std::cout << "address: " << m_address << std::endl;
	}

private:
	std::string m_name;
	std::string m_address;
};

void fun(memory_pool* pool, size_t num)
{
	auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < num; ++i)
	{
		mtx.lock();
		pool->Allocate();
		mtx.unlock();
	}
	auto end = std::chrono::steady_clock::now();
	auto diff = end - start;
	std::cout << "time elapsed : " << std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count() << " ms.." << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	memory_pool* obj_pool = new memory_pool(1000, sizeof(obj));
	
	std::thread th(fun, obj_pool, 100);
	th.join();
	return 0;
}

