#ifndef ANALYZER_H
#define ANALYZER_H

#include "common/packet.h"
#include <vector>
#include <string>

class Analyzer {
private:
    std::vector<Packet> packets;
    Statistics stats;
    
public:
    Analyzer();
    
    void addPacket(const Packet& packet);
    void analyze();
    void analyzeTopTalkers();
    int getPacketCount() const;
    bool hasErrors() const;
    Statistics generateStatistics() const;
    std::vector<Packet> getPackets() const;
    void reset();
    void displayPackets() const;
};

#endif // ANALYZER_H