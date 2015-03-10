// LockFreeMemPool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <string.h>
#include "memory_pool.h"

class foo
{
public:
	explicit foo(const std::string& str1, const std::string& str2)
	{
		memset(m_name, 0, 10);
		memset(m_address, 0, 10);
		memcpy(m_name, str1.c_str(), str1.length());
		memcpy(m_address, str2.c_str(), str2.length());
		printf("constructor is calling....\n");
	}

	void print()
	{
		printf("name: %s\n", m_name);
		printf("address: %s\n", m_address);
	}

private:
	char m_name[10];
	char m_address[10];
};

void fun(MemoryPool* pool, size_t num)
{
	std::vector<foo*> ptrs;
	for (unsigned int i = 0; i < num; i++)
	{
		foo* ptr = new(pool->Alloc())foo("fxp", "Henan");
		if (ptr != nullptr)
		{
			std::cout << "thread: " << std::this_thread::get_id() << std::endl;
			ptr->print();
		}
		ptrs.push_back(ptr);
	}

	for (int i = 0; i < ptrs.size(); ++i)
	{
		pool->Delete(ptrs[i]);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	MemoryPool* pool = new MemoryPool(100, sizeof(foo));
	std::thread th1(fun, pool, 100);
	std::thread th2(fun, pool, 40);
	th1.join();
	th2.join();

	std::cout << "we have: "<< pool->GetFreeNum() << "free blocks to use" << std::endl;
	return 0;
}

