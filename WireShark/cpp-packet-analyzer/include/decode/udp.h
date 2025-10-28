#ifndef UDP_H
#define UDP_H

#include <cstdint>
#include <string>

class UDPDecoder {
public:
    UDPDecoder(const uint8_t* data, size_t length);
    std::string decode();
    
private:
    uint16_t sourcePort;
    uint16_t destinationPort;
    uint16_t length;
    uint16_t checksum;
    const uint8_t* data;
    size_t dataLength;

    void parseHeader();
};

#endif // UDP_H