#ifndef UDP_H
#define UDP_H

#include <cstdint>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

struct UDPHeader {
    uint16_t source_port;
    uint16_t dest_port;
    uint16_t length;
    uint16_t checksum;
    
    UDPHeader() = default;
    UDPHeader(const uint8_t* data);
    
    uint16_t getSourcePort() const { return ntohs(source_port); }
    uint16_t getDestPort() const { return ntohs(dest_port); }
    uint16_t getLength() const { return ntohs(length); }
    uint16_t getChecksum() const { return ntohs(checksum); }
    
    void printHeader() const;
};

class UDPDecoder {
public:
    static bool decode(const uint8_t* data, size_t length, UDPHeader& header);
    static const uint8_t* getPayload(const uint8_t* data);
    static size_t getPayloadLength(const UDPHeader& header);
    static std::string getServiceName(uint16_t port);
};

#endif // UDP_H