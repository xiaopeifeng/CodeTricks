#include <assert.h>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

std::shared_ptr<int> kNumPtr(new int(0));
std::mutex kmtx;

std::shared_ptr<int> getSharedPtr()
{
	kmtx.lock();
	std::shared_ptr<int> ptr = kNumPtr;
	kmtx.unlock();
	return ptr;
}

void dosomething(std::shared_ptr<int> ptr)
{
	std::cout << "value: " << *ptr << std::endl;
}

int main()
{

	auto threadProc = [&](){
			for(size_t i = 0; i < 100; ++i)
			{
				kmtx.lock();
				if(!kNumPtr.unique()){
					kNumPtr.reset(new int(*kNumPtr));	
				}
				assert(kNumPtr.unique());
				*kNumPtr = *kNumPtr + 1;
				kmtx.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		};

	std::thread t1(threadProc);
	std::thread t2(threadProc);
	std::thread t3(
		[&](){
			for(size_t i = 0; i < 100000; ++i)
			{
				std::shared_ptr<int> ptr = getSharedPtr();
				dosomething(ptr);	
			}
		}
	);

	t1.join();
	t2.join();
	t3.join();
	
	std::cout << "kNumPtr's value: " << *kNumPtr << std::endl;	
	assert(*kNumPtr = 200);
	assert(kNumPtr.unique());
}
