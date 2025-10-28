#ifndef TCP_H
#define TCP_H

#include <cstdint>
#include <string>

class TCPDecoder {
public:
    TCPDecoder(const uint8_t* data, size_t length);
    std::string decode();
    
private:
    const uint8_t* data_;
    size_t length_;
    uint16_t sourcePort_;
    uint16_t destinationPort_;
    uint32_t sequenceNumber_;
    uint32_t acknowledgmentNumber_;
    uint8_t dataOffset_;
    uint8_t flags_;
    uint16_t windowSize_;
    uint16_t checksum_;
    uint16_t urgentPointer_;
    
    void parseHeader();
};

#endif // TCP_H