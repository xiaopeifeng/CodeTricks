#include "boost/bind.hpp"
#include "boost/asio.hpp"
#include "boost/asio/spawn.hpp"
#include <iostream>
#include <string>
#include <sstream>

#define MAX_BUF_LEN 1024

namespace posix = boost::asio::posix;

class say_you_love_fxp
{
public:
	say_you_love_fxp(boost::asio::io_service& io_service)
		: _input(io_service, ::dup(STDIN_FILENO))
		, _output(io_service, ::dup(STDOUT_FILENO))
		, _input_buffer(MAX_BUF_LEN)
	{}

	void begin(boost::asio::yield_context yield)
	{
		std::cout << "please say you love fxp !!!" << std::endl;
		boost::asio::async_read_until(_input, _input_buffer, '\n', yield);

		char read_buf[MAX_BUF_LEN] = { 0 };
		_input_buffer.sgetn(read_buf, _input_buffer.size() -1);
		_input_buffer.consume(_input_buffer.size());
		std::string str(read_buf);
		if (str.find("fxp") != std::string::npos && str.find("love") != std::string::npos){
			boost::asio::async_write(_output, boost::asio::buffer("what? Are u sure, you just said, you love me? \n"), yield);
		}
		std::cout << "yes ? or no" << std::endl;
		_input_buffer.consume(_input_buffer.size());
		memset(read_buf, 0, sizeof(read_buf));
		boost::asio::async_read_until(_input, _input_buffer, '\n', yield);
		_input_buffer.sgetn(read_buf, _input_buffer.size() - 1);
		str = std::string(read_buf);
		if (!str.compare("yes")){
			std::cout << "thanks, I love  you too!!!" << std::endl;
		}
		else if (!str.compare("no")){
			std::cout << "no ? I never know you!!!" << std::endl;
		}
		else{
			std::cout << "wrong input" << std::endl;
		}
	}

private:
	posix::stream_descriptor	_input;
	posix::stream_descriptor	_output;
	boost::asio::streambuf		_input_buffer;
};
int main(int argc, char** argv)
{
	boost::asio::io_service io_service;
	say_you_love_fxp say(io_service);
	boost::asio::spawn(io_service, boost::bind(&say_you_love_fxp::begin, &say, _1));

	io_service.run();
	return 0;
}

