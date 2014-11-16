#include "../pb/im.helloworld.pb.h"
#include "../serialization.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <string>

using namespace boost::asio;
boost::asio::io_service ioservice;
ip::tcp::socket sock(ioservice);	

void send_handle(const boost::system::error_code& err, std::size_t bytes_transftered)
{
	if(err)
	{
	
	}
	std::cout << "send sucess" << std::endl;
}

void connect_handle(const boost::system::error_code& err, const std::string sendbuf)
{
	if(err)	
	{
		return;	
	}
	static ip::tcp::no_delay option(true);
	sock.set_option(option);
	async_write(sock, buffer(sendbuf,sendbuf.length()),
		boost::bind(&send_handle, boost::asio::placeholders::error, 
		    boost::asio::placeholders::bytes_transferred));
}

int main()
{
	ip::tcp::endpoint peer(ip::address_v4::from_string("127.0.0.1"),27015);
	im::helloworld msg;
	msg.set_usrname("fxp");
	msg.set_passwd("fengxiaopei");
	msg.set_email("xiaopeifenng@gmail.com");
	std::string encode_message = fxp::encode(msg);
}
