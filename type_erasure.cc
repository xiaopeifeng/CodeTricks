#include <iostream>

class loader
{
public:
    virtual int size() = 0;
};

template <typename T>
class loader_imp : public loader
{
public:	
    	loader_imp(T* obj) : _real_obj(obj)
	{}

    	int size()
	{
	    return _real_obj->size();
	}
private:
	T* _real_obj;
};

class anything
{
public:
	template<typename T>
	anything(T* ptr) : _imp_ptr( new loader_imp<T>(ptr))
	{}
	int size()
	{
		_imp_ptr->size();	
	}
private:
	loader* _imp_ptr;
};

int tell_me_the_size(anything obj)
{
	return obj.size();	
}

class anything_1
{
public:
    int size()
    {
   	return 100; 
    }
};

int main()
{
    	anything_1* ptr = new anything_1();
	std::cout << tell_me_the_size(ptr);
}
