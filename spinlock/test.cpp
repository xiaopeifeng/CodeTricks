#include "Lock.h"
#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>

#define SPIN_LOCK
int sum = 0;
#ifdef SPIN_LOCK
SpinLock g_lock;
#else
std::mutex g_lock;
#endif

void func()
{
	for (int i = 0; i < 1000000; i++)
	{
		g_lock.lock();
		sum = sum + 1;
		g_lock.unlock();
	}
	std::cout << "thread: " << std::this_thread::get_id() << "finished..." << std::endl;
}

int main(int argc, char* argv[])
{
	auto start = std::chrono::steady_clock::now();
	std::thread t1(func);
	std::thread t2(func);
	std::thread t3(func);

	t1.join();
	t2.join();
	t3.join();
	auto end = std::chrono::steady_clock::now();

	std::cout << "hi, " << std::endl;
	std::cout << "After computer, the result is : ";
	std::cout << sum << std::endl;

	using namespace std::chrono;
	duration<double> time_span = \
		duration_cast<duration<double>>(end - start);
	std::cout << "time elapsed " << time_span.count() << "seconds" << std::endl;
	return 0;
}

