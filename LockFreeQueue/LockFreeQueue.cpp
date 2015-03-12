// LockFreeQueue.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Queue.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	Queue<int> q;

	for (int i = 0; i < 10; ++i)
	{
		int* p = new int(i);
		q.Push(p);
	}
	std::cout << "Queue's size: " << q.Size() << std::endl;

	int* p;
	for (int i = 0; i < 10; ++i)
	{
		bool ret = q.Pop(p);
		if (ret)
		{
			std::cout << *p << std::endl;
		}
	}

	//q.print();
	return 0;
}

