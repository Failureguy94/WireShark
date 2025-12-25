#ifndef TCP_H
#define TCP_H

#include <cstdint>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

struct TCPHeader {
    uint16_t source_port;
    uint16_t dest_port;
    uint32_t sequence;
    uint32_t acknowledgment;
    uint8_t data_offset_reserved;
    uint8_t flags;
    uint16_t window;
    uint16_t checksum;
    uint16_t urgent_ptr;
    
    TCPHeader() = default;
    TCPHeader(const uint8_t* data);
    
    uint16_t getSourcePort() const { return ntohs(source_port); }
    uint16_t getDestPort() const { return ntohs(dest_port); }
    uint32_t getSequence() const { return ntohl(sequence); }
    uint32_t getAcknowledgment() const { return ntohl(acknowledgment); }
    uint8_t getDataOffset() const { return (data_offset_reserved >> 4) * 4; }
    uint16_t getWindow() const { return ntohs(window); }
    
    // Flag methods
    bool isFIN() const { return flags & 0x01; }
    bool isSYN() const { return flags & 0x02; }
    bool isRST() const { return flags & 0x04; }
    bool isPSH() const { return flags & 0x08; }
    bool isACK() const { return flags & 0x10; }
    bool isURG() const { return flags & 0x20; }
    
    std::string getFlagsString() const;
    void printHeader() const;
};

class TCPDecoder {
public:
    static bool decode(const uint8_t* data, size_t length, TCPHeader& header);
    static const uint8_t* getPayload(const uint8_t* data, const TCPHeader& header);
    static size_t getPayloadLength(size_t totalLength, const TCPHeader& header);
    static std::string getServiceName(uint16_t port);
};

#endif // TCP_H