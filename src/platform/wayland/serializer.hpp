#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <span>
#include <concepts>
#include <type_traits>
class MessageSerializer{
    std::vector<uint8_t> buffer;

    void insertMessageLength();
    
    public:
    
    void beginMessage(uint32_t objectID, uint16_t opcode);

    void finishMessage();

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

    void appendFixed(float var);

    void appendString(const std::string& str);
    
    void appendArray(std::span<const uint8_t> arr);
    
    std::span<const uint8_t> getMessage() const;
};