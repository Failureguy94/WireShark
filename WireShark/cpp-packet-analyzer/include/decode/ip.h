#ifndef IP_H
#define IP_H

#include <cstdint>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

struct IPv4Header {
    uint8_t version_ihl;
    uint8_t tos;
    uint16_t total_length;
    uint16_t identification;
    uint16_t flags_fragment;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t source_ip;
    uint32_t dest_ip;
    
    IPv4Header() = default;
    IPv4Header(const uint8_t* data);
    
    uint8_t getVersion() const { return (version_ihl >> 4) & 0x0F; }
    uint8_t getHeaderLength() const { return (version_ihl & 0x0F) * 4; }
    uint16_t getTotalLength() const { return ntohs(total_length); }
    uint8_t getProtocol() const { return protocol; }
    std::string getSourceIP() const;
    std::string getDestIP() const;
    
    bool isTCP() const { return protocol == 6; }
    bool isUDP() const { return protocol == 17; }
    bool isICMP() const { return protocol == 1; }
    
    void printHeader() const;
};

class IPv4Decoder {
public:
    static bool decode(const uint8_t* data, size_t length, IPv4Header& header);
    static const uint8_t* getPayload(const uint8_t* data, const IPv4Header& header);
    static size_t getPayloadLength(const IPv4Header& header);
    static std::string ipToString(uint32_t ip);
    static bool validateChecksum(const IPv4Header& header);
};

#endif // IP_H