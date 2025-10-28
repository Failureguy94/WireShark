#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <cstdint>
#include <vector>

struct Packet {
    std::string timestamp;
    std::string source;
    std::string destination;
    std::string protocol;
    size_t length;
    std::string payload;
    std::vector<uint8_t> rawData;
    
    Packet() : length(0) {}
    
    bool isValid() const {
        return !rawData.empty();
    }
};

struct Statistics {
    int totalPackets = 0;
    int tcpPackets = 0;
    int udpPackets = 0;
    int icmpPackets = 0;
    int ethernetPackets = 0;
    int errorPackets = 0;
};

#endif // PACKET_H