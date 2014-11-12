#pragma once

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <string>

// create message base on message type name
google::protobuf::Message* create_message(const std::string& type_name)
{

}
