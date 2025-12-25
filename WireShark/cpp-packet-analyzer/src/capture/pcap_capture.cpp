#include <pcap.h>
#include <iostream>
#include "capture/pcap_capture.h"
#include <cstring>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>

// Global handle for pcap
static pcap_t* pcapHandle = nullptr;

PacketSniffer::PacketSniffer(const std::string& device) 
    : capturing(false), interface(device) {
    if (interface.empty()) {
        // Find default device
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_if_t* alldevs;
        if (pcap_findalldevs(&alldevs, errbuf) == 0 && alldevs != nullptr) {
            interface = alldevs->name;
            pcap_freealldevs(alldevs);
        }
    }
}

PacketSniffer::~PacketSniffer() {
    stopCapture();
}

bool PacketSniffer::startCapture() {
    char errbuf[PCAP_ERRBUF_SIZE];
    
    std::cout << "Starting packet capture on interface: " << interface << std::endl;
    
    // Open the device for capturing
    pcapHandle = pcap_open_live(
        interface.c_str(),  // device name
        65535,              // snapshot length (max packet size)
        1,                  // promiscuous mode
        1000,               // read timeout in ms
        errbuf              // error buffer
    );
    
    if (pcapHandle == nullptr) {
        std::cerr << "Error opening device: " << errbuf << std::endl;
        std::cerr << "Try running with sudo or check interface name." << std::endl;
        
        // List available interfaces
        pcap_if_t* alldevs;
        if (pcap_findalldevs(&alldevs, errbuf) == 0) {
            std::cout << "\nAvailable interfaces:" << std::endl;
            for (pcap_if_t* d = alldevs; d != nullptr; d = d->next) {
                std::cout << "  - " << d->name;
                if (d->description) std::cout << " (" << d->description << ")";
                std::cout << std::endl;
            }
            pcap_freealldevs(alldevs);
        }
        return false;
    }
    
    capturing = true;
    std::cout << "Packet capture started successfully!" << std::endl;
    return true;
}

void PacketSniffer::stopCapture() {
    capturing = false;
    if (pcapHandle != nullptr) {
        pcap_close(pcapHandle);
        pcapHandle = nullptr;
    }
    std::cout << "Packet capture stopped." << std::endl;
}

std::string ipToString(uint32_t ip) {
    struct in_addr addr;
    addr.s_addr = ip;
    return std::string(inet_ntoa(addr));
}

Packet PacketSniffer::getNextPacket() {
    Packet packet;
    
    if (!capturing || pcapHandle == nullptr) {
        return packet;
    }
    
    struct pcap_pkthdr* header;
    const u_char* data;
    
    int result = pcap_next_ex(pcapHandle, &header, &data);
    
    if (result <= 0) {
        return packet; // No packet or error
    }
    
    // Generate timestamp
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    packet.timestamp = ss.str();
    
    packet.length = header->len;
    packet.rawData.assign(data, data + header->caplen);
    
    // Parse Ethernet header
    if (header->caplen < sizeof(struct ether_header)) {
        return packet;
    }
    
    struct ether_header* ethHeader = (struct ether_header*)data;
    uint16_t etherType = ntohs(ethHeader->ether_type);
    
    // Check if it's an IP packet
    if (etherType == ETHERTYPE_IP) {
        if (header->caplen < sizeof(struct ether_header) + sizeof(struct ip)) {
            return packet;
        }
        
        struct ip* ipHeader = (struct ip*)(data + sizeof(struct ether_header));
        
        packet.source = ipToString(ipHeader->ip_src.s_addr);
        packet.destination = ipToString(ipHeader->ip_dst.s_addr);
        
        // Determine protocol
        switch (ipHeader->ip_p) {
            case IPPROTO_TCP:
                packet.protocol = "TCP";
                if (header->caplen >= sizeof(struct ether_header) + (ipHeader->ip_hl * 4) + sizeof(struct tcphdr)) {
                    struct tcphdr* tcpHeader = (struct tcphdr*)(data + sizeof(struct ether_header) + (ipHeader->ip_hl * 4));
                    uint16_t srcPort = ntohs(tcpHeader->th_sport);
                    uint16_t dstPort = ntohs(tcpHeader->th_dport);
                    
                    // Identify common services
                    if (dstPort == 80 || srcPort == 80) {
                        packet.payload = "HTTP";
                    } else if (dstPort == 443 || srcPort == 443) {
                        packet.payload = "HTTPS/TLS";
                    } else if (dstPort == 22 || srcPort == 22) {
                        packet.payload = "SSH";
                    } else {
                        packet.payload = "Port " + std::to_string(srcPort) + " -> " + std::to_string(dstPort);
                    }
                }
                break;
                
            case IPPROTO_UDP:
                packet.protocol = "UDP";
                if (header->caplen >= sizeof(struct ether_header) + (ipHeader->ip_hl * 4) + sizeof(struct udphdr)) {
                    struct udphdr* udpHeader = (struct udphdr*)(data + sizeof(struct ether_header) + (ipHeader->ip_hl * 4));
                    uint16_t srcPort = ntohs(udpHeader->uh_sport);
                    uint16_t dstPort = ntohs(udpHeader->uh_dport);
                    
                    if (dstPort == 53 || srcPort == 53) {
                        packet.payload = "DNS Query";
                    } else if (dstPort == 67 || dstPort == 68) {
                        packet.payload = "DHCP";
                    } else {
                        packet.payload = "Port " + std::to_string(srcPort) + " -> " + std::to_string(dstPort);
                    }
                }
                break;
                
            case IPPROTO_ICMP:
                packet.protocol = "ICMP";
                packet.payload = "ICMP Message";
                break;
                
            default:
                packet.protocol = "IP(" + std::to_string(ipHeader->ip_p) + ")";
                packet.payload = "Unknown";
                break;
        }
    } else if (etherType == ETHERTYPE_ARP) {
        packet.protocol = "ARP";
        packet.source = "ARP";
        packet.destination = "Broadcast";
        packet.payload = "ARP Request/Reply";
    } else {
        packet.protocol = "ETH(" + std::to_string(etherType) + ")";
        packet.source = "Unknown";
        packet.destination = "Unknown";
        packet.payload = "Non-IP Packet";
    }
    
    return packet;
}

void PacketSniffer::processPacket(const uint8_t* packet, size_t length) {
    if (length < 14) return;
    
    Packet pkt;
    pkt.length = length;
    pkt.rawData.assign(packet, packet + length);
    capturedPackets.push_back(pkt);
}

void PacketSniffer::decodePacket(const uint8_t* packet, size_t length) {
    // Decoding is now done in getNextPacket()
}