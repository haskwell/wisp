#include <stdexcept>
#include "serializer.hpp"
    
void MessageSerializer::beginMessage(uint32_t objectID, uint16_t opcode){
    buffer.clear();
    append_uint(objectID);
    append_uint(opcode);
    append_uint(static_cast<uint16_t>(0)); //message size
}

void MessageSerializer::finishMessage(){
    if(buffer.size() > 65535) throw std::length_error("Message too long");
    insertMessageLength();
}


void MessageSerializer::appendFixed(float var){
    append_int<int32_t>(var * 256);
}

void MessageSerializer::insertMessageLength(){
    uint16_t length = buffer.size();
    //write in buffer[6] and buffer[7]
    uint8_t var = length;
    buffer[6] = var;
    var = length >> 8;
    buffer[7] = var;
}

void MessageSerializer::appendString(const std::string& str){
    uint32_t length = str.length() + 1;
    append_uint(length);
    for(std::size_t i = 0; i < length - 1; i++){
        uint8_t character = str[i];
        buffer.push_back(character);
    }
    buffer.push_back('\0');
    int padding = (4 - (length % 4)) % 4;
    for(std::size_t i = 0; i < padding; i++){
        buffer.push_back(static_cast<uint8_t>(0));
    }
}

void MessageSerializer::appendArray(std::span<const uint8_t> arr){
    uint32_t length = arr.size();
    append_uint(length);
    for(std::size_t i = 0; i < length; i++){
        uint8_t character = arr[i];
        buffer.push_back(character);
    }
    int padding = (4 - (length % 4)) % 4;
    for(std::size_t i = 0; i < padding; i++){
        buffer.push_back(static_cast<uint8_t>(0));
    }
}

std::span<const uint8_t> MessageSerializer::getMessage() const{
    return buffer;
}
