#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <chrono>
#include "common/packet.h"
#include "capture/pcap_capture.h"
#include "analysis/analyzer.h"

int main() {
    std::cout << "=== Network Packet Analyzer ===" << std::endl;
    std::cout << "Mini Wireshark in C++" << std::endl;
    std::cout << "================================\n" << std::endl;

    // Create packet sniffer
    PacketSniffer sniffer;
    
    // Create analyzer
    Analyzer analyzer;

    // Start capture
    if (!sniffer.startCapture()) {
        std::cerr << "Failed to start packet capture!" << std::endl;
        return 1;
        // Capture packets for demo (10 packets)
        for (int i = 0; i < 10; i++) {
            Packet packet = sniffer.getNextPacket();
            
            if (packet.isValid()) {
                analyzer.addPacket(packet);
                
                std::cout << "[" << i+1 << "] "
                          << packet.timestamp << " | "
                          << packet.source << " -> "
                          << packet.destination << " | "
                          << packet.protocol << " | "
                          << packet.length << " bytes" << std::endl;
            }
            
            // Simulate delay
    #ifdef _WIN32
            Sleep(500); // milliseconds
    #else
            usleep(500000); // microseconds
    #endif
        }
        
        // Simulate delay
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // Stop capture
    sniffer.stopCapture();

    // Analyze captured packets
    analyzer.analyze();
    
    // Display all packets
    analyzer.displayPackets();

    std::cout << "\nCapture complete!" << std::endl;
    
    return 0;
}