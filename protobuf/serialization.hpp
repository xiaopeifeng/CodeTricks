#pragma once

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <algorithm>
#include <string>

// create message base on message type name
namespace fxp{
    google::protobuf::Message* create_message(const std::string& type_name)
    {
	google::protobuf::Message* message = NULL;
	const google::protobuf::Descriptors* descriptor =  \
	google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
	if(descriptor)
	{
	    const google::protobuf::Message* prototype = 
		google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
	    if(protobuf)
	    {
		    message = protobuf->New();	
	    }
	}
	return message;
    }

    std::string encode(const goole::protobuf::Message& message)
    {
	std::string res;
	const std::string& type_name = message.GetTypeName(); 
	size_t type_len = type_name.length();
	size_t net_len = htonl(type_len);
	res.append(reinterpret_cast<char*>(&net_len), sizeof(net_len)); // add type's length
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

    google::protobuf::Message* decode(const std::string& buf)
    {
   	if(buf.empty())	 return NULL;
	char* ptr = buf.data();
	size_t packet_len = ntohl(*( (size_t*)ptr ));
	if(buf.length() != packet_len + sizeof(size_t)) 		// check the packet's length
	{
		std::cout << "receved error buf" << std::endl;	
		return NULL;
	}
	size_t type_name_len = ntohl(*(size_t*)(ptr + size(size_t)));
	char* type_begin = ptr + 2 * sizeof(size_t);
	char* type_end = type_begin + type_name_len;
	std::string type_name(type_begin, type_end);			// get the typename
	google::protobuf::Message* result = create_message(type_name);	// create the protobuf message
	if(!result)
	{
		std::cout << "create_message error, typename is not right" << std::endl;	
		return result;
	}
	std::string packet(type_end, buf.length() - 2 * sizeof(size_t) - type_neme_len);
	if(!result->ParseFromString(packet))				// serialized from string	
	{
		delete result;	
		std::cout << "ParseFromString error" << std::endl;
		return NULL;
	}
	return result;
    }
};

