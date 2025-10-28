#ifndef PCAP_CAPTURE_H
#define PCAP_CAPTURE_H

#include <vector>
#include <cstdint>
#include <string>
#include "../common/packet.h"  // Use relative path

class PacketSniffer {
public:
    PacketSniffer(const std::string& device = "");
    ~PacketSniffer();

    bool startCapture();
    void stopCapture();
    Packet getNextPacket();
    
private:
    bool capturing;
    std::string interface;
    std::vector<Packet> capturedPackets;

    void processPacket(const uint8_t* packet, size_t length);
    void decodePacket(const uint8_t* packet, size_t length);
};

#endif // PCAP_CAPTURE_H