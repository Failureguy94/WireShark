#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <chrono>
#include <thread>
#include "common/packet.h"
#include "capture/pcap_capture.h"
#include "analysis/analyzer.h"
// #include "api/http_server.h"  // Temporarily disabled
#include "utils/logger.h"

int main()
{
    // Initialize logger
    Logger::getInstance().setLogFile("packet_analyzer.log");
    Logger::getInstance().setLogLevel(LogLevel::INFO);

    LOG_INFO("Starting Network Packet Analyzer");

    std::cout << "=== Network Packet Analyzer ===" << std::endl;
    std::cout << "Mini Wireshark in C++" << std::endl;
    std::cout << "================================\n"
              << std::endl;

    // Create packet sniffer
    PacketSniffer sniffer;

    // Create analyzer
    Analyzer analyzer;

    // Create HTTP server (disabled for now)
    // HTTPServer server(8080);

    // Set up data providers for the API
    // server.setPacketProvider([&analyzer]() { return analyzer.getPackets(); });
    // server.setStatsProvider([&analyzer]() { return analyzer.generateStatistics(); });

    // Start HTTP server
    // server.start();

    // Start capture
    if (!sniffer.startCapture())
    {
        std::cerr << "Failed to start packet capture!" << std::endl;
        return 1;
    }

    // Capture packets for demo (10 packets)
    for (int i = 0; i < 10; i++)
    {
        Packet packet = sniffer.getNextPacket();

        if (packet.isValid())
        {
            analyzer.addPacket(packet);

            std::cout << "[" << i + 1 << "] "
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

    // Stop capture
    sniffer.stopCapture();

    // Analyze captured packets
    analyzer.analyze();

    // Display all packets
    analyzer.displayPackets();

    std::cout << "\nCapture complete!" << std::endl;
    std::cout << "Basic packet analyzer demo finished." << std::endl;
    // std::cout << "HTTP API server running on http://localhost:8080" << std::endl;
    // std::cout << "Press Enter to stop..." << std::endl;

    // Wait for user input to stop
    // std::cin.get();

    // Stop server
    // server.stop();
    LOG_INFO("Network Packet Analyzer stopped");

    return 0;
}