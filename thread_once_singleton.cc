#include <pthread.h>
#include <string>
#include <iostream>

class Singleton
{
public:
	static Singleton* getInstance()
	{
		pthread_once(&onceFlag_, init);
		return obj_;
	}

	std::string function()
	{
		return "hi, there.\n";
	}

private:
	Singleton() {}
	~Singleton(){}
	Singleton(const Singleton&);
	void operator=(const Singleton&);

	static void init()
	{
		obj_ = new Singleton();
	}	
	
private:
	static pthread_once_t onceFlag_;
	static Singleton* obj_;
};

pthread_once_t Singleton::onceFlag_ = PTHREAD_ONCE_INIT;
Singleton* Singleton::obj_ = NULL;

int main()
{
	Singleton* ptr = Singleton::getInstance();
	std::cout << ptr->function();
}
