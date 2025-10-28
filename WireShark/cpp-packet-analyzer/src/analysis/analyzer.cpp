#include "analysis/analyzer.h"
#include <iostream>
#include <algorithm>
#include <map>

Analyzer::Analyzer() {
    stats = Statistics();
}

void Analyzer::addPacket(const Packet& packet) {
    packets.push_back(packet);
    stats.totalPackets++;
    
    if (packet.protocol == "TCP") {
        stats.tcpPackets++;
    } else if (packet.protocol == "UDP") {
        stats.udpPackets++;
    } else if (packet.protocol == "ICMP") {
        stats.icmpPackets++;
    }
}

void Analyzer::analyze() {
    std::cout << "\n=== Packet Analysis ===" << std::endl;
    std::cout << "Total Packets: " << stats.totalPackets << std::endl;
    std::cout << "TCP Packets: " << stats.tcpPackets << std::endl;
    std::cout << "UDP Packets: " << stats.udpPackets << std::endl;
    std::cout << "ICMP Packets: " << stats.icmpPackets << std::endl;
    
    analyzeTopTalkers();
}

void Analyzer::analyzeTopTalkers() {
    std::map<std::string, int> ipCount;
    
    for (const auto& packet : packets) {
        ipCount[packet.source]++;
        ipCount[packet.destination]++;
    }
    
    std::cout << "\n=== Top Talkers ===" << std::endl;
    for (const auto& pair : ipCount) {
        std::cout << pair.first << " : " << pair.second << " packets" << std::endl;
    }
}

int Analyzer::getPacketCount() const {
    return packets.size();
}

bool Analyzer::hasErrors() const {
    return stats.errorPackets > 0;
}

Statistics Analyzer::generateStatistics() const {
    return stats;
}

void Analyzer::reset() {
    packets.clear();
    stats = Statistics();
}

void Analyzer::displayPackets() const {
    std::cout << "\n=== Captured Packets ===" << std::endl;
    std::cout << "Time\t\t\tSource\t\t\tDestination\t\tProtocol\tLength" << std::endl;
    std::cout << "------------------------------------------------------------------------" << std::endl;
    
    for (const auto& packet : packets) {
        std::cout << packet.timestamp << "\t"
                  << packet.source << "\t\t"
                  << packet.destination << "\t\t"
                  << packet.protocol << "\t\t"
                  << packet.length << std::endl;
    }
}