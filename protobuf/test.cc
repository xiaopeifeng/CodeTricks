#include "serialization.hpp"
#include "pb/im.helloworld.pb.h"
#include <string>
#include <iostream>
#include <google/protobuf/message.h>

int main()
{
    	im::helloworld msg;
	msg.set_usrname("fxp");
	msg.set_passwd("fengxiaopei");
	msg.set_email("xiaopeifenng@gmail.com");

	std::string encode_str = fxp::encode(msg);

	im::helloworld* msg2;
	google::protobuf::Message* ptr;
	ptr = fxp::decode(encode_str);
	msg2 = dynamic_cast<im::helloworld* >(ptr);
	std::cout << ptr->GetTypeName() << std::endl;	
	std::cout << msg2->usrname() << std::endl;
	std::cout << msg2->passwd() << std::endl;
	std::cout << msg2->email() << std::endl;
}
