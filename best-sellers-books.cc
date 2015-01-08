#include "avhttp.hpp"
#include <regex>
#include <iostream>

class best_seller
{
public:
	explicit best_seller(const boost::asio::io_service& io)
			: _io(io)
			, _h(io)
	{}
	
	~bestsellers()
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
		std::istream is(_h);
		std::string str;		
		while(is.good())
		{
			str.clear();
			std::getline(is, str);
			switch(_state)
			{
			case state_unknown:
				if(str.find())
			}
		}
	}

private:
	enum {
		state_unknown;
		state_found;
	}_state;

private:
	std::string _url;		
	boost::asio::io_service _io;
	avhttp::http_stream _h;
};

int main(int argc, char** argv)
{
	boost::asio::io_service io;
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
}

