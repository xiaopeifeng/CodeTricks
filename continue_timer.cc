#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>

boost::asio::io_service ios;
void continue_timer();
// how to use it
// just push the continue_timer into where you want do something every some seconds
void something_to_do(void)
{
    //	things to do every 5 seconds
	std::cout << "i love U..." << std::endl;
	std::cout << "see u 5 seconds later..." << std::endl;
	continue_timer();
}

boost::asio::deadline_timer timer(ios);			

void continue_timer()
{
	static boost::asio::deadline_timer timer(ios);			
	timer.expires_from_now(boost::posix_time::seconds(5));
	timer.async_wait(boost::bind(&something_to_do));
}

int main()
{
	continue_timer();
	ios.run();
}

