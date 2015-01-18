#include "avhttp.hpp"
#include "boost/algorithm/string.hpp"
#include <regex>
#include <iostream>

class best_seller
{
public:
	explicit best_seller(boost::asio::io_service& io)
			: _io(io)
			, _h(io)
	{}
	
	~best_seller()
	{}

public:
	void start(void)
	{
		boost::system::error_code ec;	
		std::string query_url = "http://bang.dangdang.com/books/bestsellers/01.00.00.00.00.00-recent7-0-0-1-1";
		_h.open(query_url, ec);
		if(ec){
			std::cerr << "cannot open the url" << std::endl;	
			return;
		}
		handle_process();
	}

private:
	void handle_process()
	{
		std::istream is(&_h);
		std::string str;
		while(std::getline(is, str)) {
			if(str.find("bang_list clearfix bang_list_mode")) break;	
		}

		while(is.good())
		{
			str.clear();
			std::getline(is, str);
			switch(_state)
			{
			case state_unknown:
				boost::trim(str);
				if(str.find("div class=\"list_num red\"")){
					_state = state_skip_1;		
				}
				break;
			case state_skip_1: _state = state_found;break;
			case state_found:
					std::cout << str << std::endl;
					_state = state_unknown;
			default: break;
			}
		}
	}

private:
	enum {
		state_unknown,
		state_skip_1,
		state_found
	}_state;

private:
	std::string _url;		
	boost::asio::io_service& _io;
	avhttp::http_stream _h;
};

int main(int argc, char** argv)
{
	boost::asio::io_service io;
	/*
	boost::system::error_code ec;
	avhttp::http_stream h(io);
	h.open(query_url, ec);
	if(ec){
		std::cerr << "cannot open the url" << std::endl;	
		return -1;
	}
	std::string result;
	std::istream is(&h);
	
	try
	{
		while(is.good())	
		{
			std::string str;
			std::getline(is, str);	
			result += str;
		}
		std::cout << result;
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;	
		return -1;
	}
	*/
	best_seller book(io);
	book.start();

	io.run();
}

