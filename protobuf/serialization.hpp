#pragma once

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <cstdlib>

// create message base on message type name
namespace fxp{
    static google::protobuf::Message* create_message(const std::string& type_name)
    {
	google::protobuf::Message* message = NULL;
	const google::protobuf::Descriptor* descriptor =  \
	google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
	if(descriptor)
	{
	    const google::protobuf::Message* prototype = 
		google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
	    if(prototype)
	    {
		    message = prototype->New();	
	    }
	}
	return message;
    }

    static std::string encode(const google::protobuf::Message& message)
    {
	std::string res;
	const std::string& type_name = message.GetTypeName(); 
	size_t type_len = type_name.length();
	size_t net_len = htonl(type_len);
	res.resize(sizeof(size_t));
	res.append(reinterpret_cast<char*>(&net_len), sizeof(size_t)); // add type's length
	res.append(type_name.c_str(), type_len);			// add type's type
	bool success = message.AppendToString(&res);
	if(success)
	{
		size_t len = htonl(res.length());	
		std::copy(reinterpret_cast<char*>(&len), reinterpret_cast<char*>(&len) + sizeof(len),
			res.begin());
	}
	else
	{
		std::cout << "encode error" << std::endl;	
		res.clear();
	}
	return res;
    }

    static google::protobuf::Message* decode(const std::string& buf)
    {
   	if(buf.empty())	 return NULL;
	const char* ptr = buf.data();
	size_t packet_len = ntohl(*( (size_t*)ptr ));
	std::cout << "buf.length: " << buf.length() << std::endl;
	std::cout << "packet_len: " << packet_len << std::endl;
	if(buf.length() != packet_len) 		// check the packet's length
	{
		std::cout << "receved error buf" << std::endl;	
		return NULL;
	}
	size_t type_name_len = ntohl(*(size_t*)(ptr + sizeof(size_t)));
	const char* type_begin = ptr + 2 * sizeof(size_t);
	const char* type_end = type_begin + type_name_len;
	std::string type_name(type_begin, type_end);			// get the typename
	std::cout << "type_name_len: " << type_name_len << std::endl;
	std::cout << "type_name: " << type_name << std::endl;
	google::protobuf::Message* result = create_message(type_name);	// create the protobuf message
	if(!result)
	{
		std::cout << "create_message error, typename is not right" << std::endl;	
		return result;
	}
	std::string packet(type_end, buf.length() - 2 * sizeof(size_t) - type_name_len);
	if(!result->ParseFromString(packet))				// serialized from string	
	{
		delete result;	
		std::cout << "ParseFromString error" << std::endl;
		return NULL;
	}
	return result;
    }
};

