#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/unordered_set.hpp>

#include <unistd.h>
#include <iostream>
#define GIRL_NUM 5

class foo;
struct Girl;

void DumpFriendList();
std::vector<boost::weak_ptr<Girl> > girls;
boost::circular_buffer<boost::unordered_set<boost::shared_ptr<foo> > > circular_buf(5);
boost::mutex mtx;

struct Girl
{
	Girl(int i) : m_id(i) {}

	void Say()
	{
		std::cout << "id: " << m_id << " say: " << "i love u, fxp!" << std::endl;
	}

	void SaySorry()
	{
		std::cout << "id: " << m_id << " say: " << "i am sorry, i am late..." << std::endl;
	}

	int m_id;
	boost::weak_ptr<foo> m_ref;
};

// some girl say "i love u, fxp" , then she will live more 5 seconds
// if you do not say, you will be deleted after 5 seconds 
void Saylove()
{
	srand(NULL);
	int times = 0;
	for (;;)
	{
		int i = rand() % GIRL_NUM;
		girls[i].lock()->Say();
		boost::weak_ptr<foo> w_ptr = girls[i].lock()->m_ref;
		boost::shared_ptr<foo> ptr(w_ptr.lock());
		{
			boost::lock_guard<boost::mutex> guard(mtx);
			circular_buf.back().insert(ptr);
		}
		sleep(1);
		times++;
		if(times == 5) break;
	}
}

class foo
{
public:
	foo( const boost::weak_ptr<Girl> &ptr )
		: m_friend(ptr)
	{}

	~foo()
	{
		boost::shared_ptr<Girl> ptr = m_friend.lock();
		if (ptr)
		{
			ptr->SaySorry();
		}
	}

	bool operator < (const foo &rfs) const
	{
		return true;
	}

private:
	boost::weak_ptr<Girl> m_friend;
};

//this is the judger, he can judge if you have said "i love u, fxp" in 5 seconds
//if not, then you will be move out.
void Judger()
{
	while (true)
	{
		boost::unordered_set<boost::shared_ptr<foo> > s;
		{
			boost::lock_guard<boost::mutex> guard(mtx);
		//	DumpFriendList();
			circular_buf.push_back(s);
		}
		
		sleep(1);
	}
}

void DumpFriendList()
{
	std::cout << "size = " << circular_buf.size() << std::endl;
	typedef boost::circular_buffer<boost::unordered_set<boost::shared_ptr<foo> > > BUFF;
	int i = 0;
	for(BUFF::iterator iter = circular_buf.begin(); iter != circular_buf.end(); ++iter,++i)
	{
		boost::unordered_set<boost::shared_ptr<foo> > &ref = *iter;
		std::cout << i << " len : " << ref.size() << std::endl;
		boost::unordered_set<boost::shared_ptr<foo> >::iterator iterj = ref.begin();
		for(; iterj != ref.end(); ++iterj)
		{
			std::cout << "usecount: " << iterj->use_count() << std::endl;
		}
	}
}

int main(int argc, char* argv[])
{
	//test five girl
	boost::shared_ptr<Girl> ptr1(new Girl(1));
	boost::shared_ptr<Girl> ptr2(new Girl(2));
	boost::shared_ptr<Girl> ptr3(new Girl(3));
	boost::shared_ptr<Girl> ptr4(new Girl(4));
	boost::shared_ptr<Girl> ptr5(new Girl(5));

	boost::weak_ptr<Girl> w1(ptr1);
	boost::weak_ptr<Girl> w2(ptr2);
	boost::weak_ptr<Girl> w3(ptr3);
	boost::weak_ptr<Girl> w4(ptr4);
	boost::weak_ptr<Girl> w5(ptr5);

	girls.push_back(w1);
	girls.push_back(w2);
	girls.push_back(w3);
	girls.push_back(w4);
	girls.push_back(w5);

	{	
	boost::shared_ptr<foo> foo1(new foo(w1)); 
	boost::weak_ptr<foo> w_ptr1(foo1);
	girls[0].lock()->m_ref = w_ptr1;
	std::cout << "foo1.usecount: " << foo1.use_count() << std::endl;
	boost::shared_ptr<foo> foo2(new foo(w2));
	boost::weak_ptr<foo> w_ptr2(foo2);
	girls[1].lock()->m_ref = w_ptr2;

	boost::shared_ptr<foo> foo3(new foo(w3));
	boost::weak_ptr<foo> w_ptr3(foo3);
	girls[2].lock()->m_ref = w_ptr3;

	boost::shared_ptr<foo> foo4(new foo(w4));
	boost::weak_ptr<foo> w_ptr4(foo4);
	girls[3].lock()->m_ref = w_ptr4;

	boost::shared_ptr<foo> foo5(new foo(w5));
	boost::weak_ptr<foo> w_ptr5(foo5);
	girls[4].lock()->m_ref = w_ptr5;
	
	circular_buf.resize(5);

	std::cout << "foo1.usecount: " << foo1.use_count() << std::endl;
	circular_buf.back().insert(foo1);
	circular_buf.back().insert(foo2);
	circular_buf.back().insert(foo3);
	circular_buf.back().insert(foo4);
	circular_buf.back().insert(foo5);
	}
	

	boost::thread t(Judger);
	boost::thread t2(Saylove);

	t.join();
	t2.join();

	return 0;
}
