#include <iostream>
#include <chrono>
#include <thread>

#include "compiler_micro.h"

#define TEST_TIMES 100000000

struct CACHELINE_ALIGNMENT Foo {
	long a;
	long b;
};

struct Foo2 {
	long x;
	long y;
};

long a;
Foo fc;

long b;
Foo2 fc2;

void func_read(long *ptr)
{
	long x = 0;
	for (auto i = 0; i < TEST_TIMES; ++i) {
		x += *ptr;
	}
}

void func_write(long *ptr)
{
	for (auto i = 0; i < TEST_TIMES; ++i) {
		*ptr += 1;
	}
}

int main(int argc, char** argv) {
	typedef std::chrono::high_resolution_clock Time;
	typedef std::chrono::milliseconds ms;
	typedef std::chrono::duration<float> fsec;

	auto start = Time::now();
	if (std::atoi(argv[1]) == 0) {
		std::thread t1(std::bind(func_read, &(fc.a)));
		std::thread t2(std::bind(func_write, &a));
		t1.join();t2.join();
	} else {
		std::thread t1(std::bind(func_read, &(fc2.x)));
		std::thread t2(std::bind(func_write, &b));
		t1.join();t2.join();
	}

	auto end = Time::now();
	fsec fs = end - start;
	auto fs2 = std::chrono::duration_cast<ms>(fs);
	std::cout << "finished\n";
	std::cout << fs.count() << "s\n";
	std::cout << fs2.count() << "ms\n";
}

