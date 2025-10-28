#include "udp.h"
#include <iostream>
#include <pcap.h>

class UDPDecoder {
public:
    static void decode(const u_char* packet, int size) {
        if (size < sizeof(udphdr)) {
            std::cerr << "Packet size is too small to contain UDP header." << std::endl;
            return;
        }

        const udphdr* udpHeader = reinterpret_cast<const udphdr*>(packet);
        std::cout << "UDP Source Port: " << ntohs(udpHeader->source) << std::endl;
        std::cout << "UDP Destination Port: " << ntohs(udpHeader->dest) << std::endl;
        std::cout << "UDP Length: " << ntohs(udpHeader->len) << std::endl;
        std::cout << "UDP Checksum: " << ntohs(udpHeader->check) << std::endl;

        // Additional decoding logic can be added here
    }
};