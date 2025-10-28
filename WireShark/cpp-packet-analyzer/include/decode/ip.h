#ifndef IP_H
#define IP_H

#include <cstdint>
#include <string>

class IPHeader {
public:
    IPHeader(const uint8_t* data);
    std::string getSourceIP() const;
    std::string getDestinationIP() const;
    uint8_t getProtocol() const;
    uint16_t getTotalLength() const;

private:
    uint8_t versionAndIHL;
    uint8_t typeOfService;
    uint16_t totalLength;
    uint16_t identification;
    uint16_t flagsAndFragmentOffset;
    uint8_t timeToLive;
    uint8_t protocol;
    uint16_t headerChecksum;
    uint32_t sourceIP;
    uint32_t destinationIP;

    void parseHeader(const uint8_t* data);
};

#endif // IP_H