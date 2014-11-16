#include <map>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "../serialization.hpp"
#include "../pb/im.helloworld.pb.h"

using namespace boost::asio;

class server
{
    typedef boost::function<void(google::protobuf::Message*)> message_callback_t;
public:
    server(boost::asio::io_service& io_service, short port)
	_ioservice(io_service),
	_acceptor(io_service, ip::tcp::endpoint(ip::tcp::v4(), port))
	{}
public:
    	void start()
	{
	    	auto sock = boost::make_shared<ip::tcp::socket>(boost::ref(_ioservice));
		_acceptor.async_accept(sock, boost::bind(hand_accept,));	
	}

    	void add_message_process_mode(const std::string& name, message_callback_t cb)
    	{
   		_message_callbacks[name] = cb; 
    	}
private:
    	void handle_accept(ip::tcp::socket* sock, boost::system::error_code err)
	{
		if(err) return;
		start();
		std::cout << "client: " << sock->remote_endpoint().address().to_string() << "is connected..."  << std::endl;
		memset(_buf, 0 ,max_length);
		sock->async_read_some(boost::asio::buffer(_buf, max_length),
			boost::bind(&server::handle_read, this, 
			    boost::asio::placeholders::error,
			    boost::asio::placeholders::bytes_transferred));
			
	}

	void handle_read(const boost::system::error& err, size_t bytes_transferred)
	{
		if(err) return;
		google::protobuf::Message* message = fxp::decode();
		deal_message();	
	}

	void deal_message(google::protobuf::Message* msg)
	{
		auto iter_map = _message_callbacks.find(msg->GetTypeName());
		if(iter_map != _message_callbacks.end())
		{
			iter_map->second(msg);	
		}
	}
private:
	boost::asio::io_service _ioservice;
	ip::tcp::acceptor _acceptor;
	std::map<std::string, message_callback_t> _message_callbacks;
	enum {max_length = 1024};
	char _buf[max_length];
};

void deal_with_helloworld(google::protobuf::Message* msg)
{
	im::helloword* msg_deal = dynamic_cast<im::helloworld*>(msg);
	if(msg_deal)
	{
		std::cout << msg_deal->GetTypeName() << std::endl;	
		std::cout << msg_deal->usrname() << std::endl;
		std::cout << msg_deal->passwd() << std::endl;
		std::cout << msg_deal->email() << std::endl;
	}
}

int main()
{
    	boost::asio::io_service ioservice;
	short port = 27015;
    	server s(ioservice, port);
	s.add_message_process_mode("im.helloworld", 
		boost::bind(*deal_with_helloworld, 
		    _1));
	s.start();
	ioservice.run();
}
