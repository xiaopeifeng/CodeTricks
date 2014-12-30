#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <iostream>
#include <string>

#define MAX_BUF_LEN 1024

namespace posix = boost::asio::posix;

class say_you_love_fxp
{
public:
	say_you_love_fxp(boost::asio::io_service& io_service)
		: _input(io_service, ::dup(STDIN_FILENO))
		, _output(io_service, ::dup(STDOUT_FILENO))
		, _input_buffer(MAX_BUF_LEN)
	{
	}

	void begin()
	{
		boost::asio::async_read_until(_input, _input_buffer, '\n',
			boost::bind(&say_you_love_fxp::handle_read_from_cin, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));	
	}

private:
	void handle_read_from_cin(const boost::system::error_code& error, std::size_t length)
	{
		if(!error)	
		{
			char read_buf[MAX_BUF_LEN] = {0};
			_input_buffer.sgetn(read_buf, length - 1);
			_input_buffer.consume(1);
			std::string str(read_buf);
			if(str.find("fxp") != std::string::npos && str.find("love") != std::string::npos){
				boost::asio::async_write(_output, boost::asio::buffer("What? Are u sure, you just said, you love me? ...\n"),
					boost::bind(&say_you_love_fxp::handle_write_to_console, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));	
			}
		}
	}

	void handle_write_to_console(const boost::system::error_code& error, std::size_t length)
	{
		if(!error)
		{
			std::cout << "yes? or no" << std::endl;
			boost::asio::async_read_until(_input, _input_buffer, '\n',
				boost::bind(&say_you_love_fxp::handle_input_yes_no, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
		}
	}

	void handle_input_yes_no(const boost::system::error_code& error, std::size_t length)
	{
		if(!error)	
		{
			char read_buf[MAX_BUF_LEN] = {0};
			_input_buffer.sgetn(read_buf, length - 1);
			_input_buffer.consume(1);
			std::string str(read_buf);
			if(!str.compare("yes")){
				std::cout << "i love u too..." << std::endl;	
			}else if(!str.compare("no")){
				std::cout << "i never know you..." << std::endl;	
			}else{
				std::cout << "yes or no?" << std::endl;	
			}
		}
	}

private:
	posix::stream_descriptor	_input;
	posix::stream_descriptor	_output;
	boost::asio::streambuf		_input_buffer;
};

int main()
{
	boost::asio::io_service io_service;
	say_you_love_fxp say_love(io_service);
	say_love.begin();	
		
	io_service.run();
}
