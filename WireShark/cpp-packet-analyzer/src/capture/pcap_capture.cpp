#include <pcap.h>
#include <iostream>
#include "capture/pcap_capture.h"
#include "decode/ethernet.h"
#include "decode/ip.h"
#include "decode/tcp.h"
#include "decode/udp.h"
#include <cstring>
#include <chrono>
#include <iomanip>
#include <sstream>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <iphlpapi.h>
    #pragma comment(lib, "ws2_32.lib")
    #pragma comment(lib, "iphlpapi.lib")
#endif

PacketSniffer::PacketSniffer(const std::string& device) 
    : capturing(false), interface(device) {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

PacketSniffer::~PacketSniffer() {
    stopCapture();
#ifdef _WIN32
    WSACleanup();
#endif
}

bool PacketSniffer::startCapture() {
    std::cout << "Starting packet capture..." << std::endl;
    capturing = true;
    return true;
}

void PacketSniffer::stopCapture() {
    capturing = false;
    std::cout << "Packet capture stopped." << std::endl;
}

Packet PacketSniffer::getNextPacket() {
    Packet packet;
    
    if (!capturing) {
        return packet;
    }
    
    // Generate timestamp
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    packet.timestamp = ss.str();
    
    // Dummy packet for testing
    static int counter = 0;
    packet.source = "192.168.1." + std::to_string(100 + (counter % 5));
    packet.destination = counter % 2 == 0 ? "8.8.8.8" : "1.1.1.1";
    packet.protocol = counter % 3 == 0 ? "UDP" : "TCP";
    packet.length = 64 + (counter * 10);
    packet.payload = packet.protocol == "TCP" ? "HTTP GET Request" : "DNS Query";
    packet.rawData.resize(packet.length, 0);
    counter++;
    
    return packet;
}

void PacketSniffer::processPacket(const uint8_t* packet, size_t length) {
    if (length < 14) return;
    
    Packet pkt;
    pkt.length = length;
    pkt.rawData.assign(packet, packet + length);
    
    decodePacket(packet, length);
    capturedPackets.push_back(pkt);
}

void PacketSniffer::decodePacket(const uint8_t* packet, size_t length) {
    if (length < 14) return;
    
    uint16_t etherType = (packet[12] << 8) | packet[13];
    
    if (etherType == 0x0800 && length >= 34) {
        const uint8_t* ipHeader = packet + 14;
        uint8_t protocol = ipHeader[9];
        
        std::cout << "Packet captured - Protocol: ";
        
        if (protocol == 6) {
            std::cout << "TCP" << std::endl;
        } else if (protocol == 17) {
            std::cout << "UDP" << std::endl;
        } else if (protocol == 1) {
            std::cout << "ICMP" << std::endl;
        } else {
            std::cout << "Other (" << (int)protocol << ")" << std::endl;
        }
    }
}