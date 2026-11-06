#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <span>
class MessageSerializer{
    std::vector<uint8_t> buffer;
    public:
    
    void beginMessage(uint32_t objectID, uint16_t opcode){
        buffer.clear();
        append_uint(objectID);
        append_uint(opcode);
        append_uint(static_cast<uint16_t>(0)); //message size
    }

    void finishMessage(){
        if(buffer.size() > 65535) throw std::length_error("Message too long");
        insertMessageLength();
    }

    template <std::unsigned_integral T>
    void append_uint(T var){
        for(std::size_t i = 0; i < sizeof(T); i++){
            uint8_t extractedValue = var;
            buffer.push_back(extractedValue);
            var = var >> 8;
        }
    }

    template <std::signed_integral T>
    void append_int(T var){
        using U = std::make_unsigned_t<T>;
        U raw = var;
        for(std::size_t i = 0; i < sizeof(T); i++){
            uint8_t extractedValue = raw;
            buffer.push_back(extractedValue);
            raw = raw >> 8;
        }
    }

    void insertMessageLength(){
        uint16_t length = buffer.size();
        //write in buffer[6] and buffer[7]
        uint8_t var = length;
        buffer[6] = var;
        var = length >> 8;
        buffer[7] = var;
    }

    void appendString(const std::string& str){
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
    
    void appendArray(std::span<const uint8_t> arr){
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

    std::span<const uint8_t> getMessage(){
        return buffer;
    }
};