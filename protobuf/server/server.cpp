#include <map>
#include <iostream>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <google/protobuf/message.h>
#include "../serialization.hpp"
#include "../pb/im.helloworld.pb.h"

using namespace boost::asio;
io_service ioservice;

class server : public boost::enable_shared_from_this<server>
{
    typedef boost::function<void(google::protobuf::Message*)> message_callback_t;
public:
    	server(short port) 
	    : _acceptor(ioservice, ip::tcp::endpoint(ip::tcp::v4(), port))
	{}
public:
    	void start()
	{
	    ip::tcp::socket* sock = new ip::tcp::socket(ioservice);
	    _acceptor.async_accept(*sock, boost::bind(&server::handle_accept, this, 
			    				sock, 
			    				boost::asio::placeholders::error));	
	}

    	void add_message_process_mode(const std::string& name, message_callback_t cb)
    	{
   		_message_callbacks[name] = cb; 
    	}
private:
    	void handle_accept(ip::tcp::socket* sock, const boost::system::error_code& err)
	{
		if(err) return;
		start();
		std::cout << "client: " << sock->remote_endpoint().address().to_string() << "is connected..."  << std::endl;
		memset(_buf, 0, max_length);
		int packet_length;
		boost::asio::read(*sock, boost::asio::buffer(&packet_length, sizeof(int)));
		std::cout << "length : " << packet_length << std::endl;

		boost::asio::async_read(*sock, request_buf, boost::asio::transfer_exactly(packet_length), 
			boost::bind(&server::handle_read, this,
			    boost::asio::placeholders::error,
			    boost::asio::placeholders::bytes_transferred));
	}

	void handle_read(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if(err) return;
	    	std::cout << bytes_transferred << "bytes received..." << std::endl;
		std::string tmp_str; tmp_str.resize(bytes_transferred);
		request_buf.sgetn(&tmp_str[0], bytes_transferred);
		request_buf.consume(bytes_transferred);
		google::protobuf::Message* message = fxp::decode(tmp_str);
		std::cout << "msg type: " << message->GetTypeName();
		deal_message(message);	
	}

	void deal_message(google::protobuf::Message* msg)
	{
		auto iter_map = _message_callbacks.find(msg->GetTypeName());
		if(iter_map != _message_callbacks.end())
		{
			iter_map->second(msg);	
		}
		else
		{
			std::cout << "message dealer no found" << std::endl;	
		}
	}
private:
	ip::tcp::acceptor 				_acceptor;
	std::map<std::string, message_callback_t> 	_message_callbacks;
	enum 						{max_length = 1024};
	char 						_buf[max_length];
	boost::asio::streambuf				request_buf;
};

void deal_with_helloworld(google::protobuf::Message* msg)
{
	im::helloworld* msg_deal = dynamic_cast<im::helloworld* >(msg);
	if(msg_deal)
	{
		std::cout << msg_deal->GetTypeName() << std::endl;	
		std::cout << msg_deal->usrname() << std::endl;
		std::cout << msg_deal->passwd() << std::endl;
		std::cout << msg_deal->email() << std::endl;
	}
	delete msg_deal;
}

int main()
{
	short port = 27015;
    	server s(port);
	s.add_message_process_mode("im.helloworld", 
		boost::bind(&deal_with_helloworld, 
		    _1));
	s.start();

	ioservice.run();
}
