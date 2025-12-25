#include "ui/tui.h"
#include <iostream>

void TUI::displayPacketInfo(const Packet& packet) {
    std::cout << "Packet Information:" << std::endl;
    std::cout << "Timestamp: " << packet.timestamp << std::endl;
    std::cout << "Source: " << packet.source << std::endl;
    std::cout << "Destination: " << packet.destination << std::endl;
    std::cout << "Protocol: " << packet.protocol << std::endl;
    std::cout << "Length: " << packet.length << " bytes" << std::endl;
    std::cout << "Payload: " << packet.payload << std::endl;
    std::cout << "-----------------------------------" << std::endl;
}

void TUI::displayStatistics(const Statistics& stats) {
    std::cout << "Statistics:" << std::endl;
    std::cout << "Total Packets: " << stats.totalPackets << std::endl;
    std::cout << "TCP Packets: " << stats.tcpPackets << std::endl;
    std::cout << "UDP Packets: " << stats.udpPackets << std::endl;
    std::cout << "Ethernet Packets: " << stats.ethernetPackets << std::endl;
    std::cout << "-----------------------------------" << std::endl;
}

void TUI::clearScreen() {
    std::cout << "\033[2J\033[1;1H"; // ANSI escape code to clear the screen
}