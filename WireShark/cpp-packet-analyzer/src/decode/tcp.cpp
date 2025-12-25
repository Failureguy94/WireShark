#include "decode/tcp.h"
#include <iostream>
#include <netinet/tcp.h>

TCPDecoder::TCPDecoder(const u_char* data, size_t length) {
    if (length < sizeof(tcphdr)) {
        std::cerr << "TCP packet is too short" << std::endl;
        return;
    }
    const tcphdr* tcpHeader = reinterpret_cast<const tcphdr*>(data);
    sourcePort = ntohs(tcpHeader->source);
    destinationPort = ntohs(tcpHeader->dest);
    sequenceNumber = ntohl(tcpHeader->seq);
    acknowledgmentNumber = ntohl(tcpHeader->ack_seq);
    dataOffset = tcpHeader->doff * 4; // TCP header length in bytes
    flags = tcpHeader->fin | (tcpHeader->syn << 1) | (tcpHeader->rst << 2) |
            (tcpHeader->psh << 3) | (tcpHeader->ack << 4) | (tcpHeader->urg << 5);
    windowSize = ntohs(tcpHeader->window);
}

void TCPDecoder::print() const {
    std::cout << "TCP Packet:" << std::endl;
    std::cout << "  Source Port: " << sourcePort << std::endl;
    std::cout << "  Destination Port: " << destinationPort << std::endl;
    std::cout << "  Sequence Number: " << sequenceNumber << std::endl;
    std::cout << "  Acknowledgment Number: " << acknowledgmentNumber << std::endl;
    std::cout << "  Data Offset: " << dataOffset << std::endl;
    std::cout << "  Flags: " << flags << std::endl;
    std::cout << "  Window Size: " << windowSize << std::endl;
}