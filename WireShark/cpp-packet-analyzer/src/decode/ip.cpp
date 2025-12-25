#include "decode/ip.h"
#include <arpa/inet.h>

IPHeader::IPHeader(const u_char* data) {
    // Copy the IP header data from the packet
    memcpy(&header, data, sizeof(header));
}

std::string IPHeader::getSourceIP() const {
    char buffer[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &header.ip_src, buffer, sizeof(buffer));
    return std::string(buffer);
}

std::string IPHeader::getDestinationIP() const {
    char buffer[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &header.ip_dst, buffer, sizeof(buffer));
    return std::string(buffer);
}

uint8_t IPHeader::getProtocol() const {
    return header.ip_p;
}

uint16_t IPHeader::getTotalLength() const {
    return ntohs(header.ip_len);
}