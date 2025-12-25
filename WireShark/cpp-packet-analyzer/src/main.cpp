#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <chrono>
#include <csignal>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include "common/packet.h"
#include "capture/pcap_capture.h"
#include "analysis/analyzer.h"
#include "utils/logger.h"

// Global state
std::atomic<bool> running(true);
std::mutex packetMutex;
std::vector<Packet> capturedPackets;
Statistics globalStats;

void signalHandler(int signal) {
    std::cout << "\nShutting down..." << std::endl;
    running = false;
}

// Simple HTTP server for the React frontend
void runHttpServer(int port) {
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) {
        LOG_ERROR("Failed to create HTTP socket");
        return;
    }
    
    int opt = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        LOG_ERROR("Failed to bind HTTP server to port " + std::to_string(port));
        close(serverFd);
        return;
    }
    
    if (listen(serverFd, 10) < 0) {
        LOG_ERROR("Failed to listen on HTTP server");
        close(serverFd);
        return;
    }
    
    LOG_INFO("HTTP API Server running on http://localhost:" + std::to_string(port));
    std::cout << "HTTP API Server running on http://localhost:" << port << std::endl;
    
    while (running) {
        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(serverFd, &readfds);
        
        int activity = select(serverFd + 1, &readfds, NULL, NULL, &tv);
        
        if (activity <= 0) continue;
        
        struct sockaddr_in clientAddr{};
        socklen_t addrLen = sizeof(clientAddr);
        int clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &addrLen);
        
        if (clientFd < 0) continue;
        
        char buffer[4096] = {0};
        read(clientFd, buffer, sizeof(buffer));
        
        std::string request(buffer);
        std::string response;
        std::string body;
        
        // CORS headers
        std::string corsHeaders = 
            "Access-Control-Allow-Origin: *\r\n"
            "Access-Control-Allow-Methods: GET, OPTIONS\r\n"
            "Access-Control-Allow-Headers: Content-Type\r\n";
        
        if (request.find("OPTIONS") == 0) {
            response = "HTTP/1.1 204 No Content\r\n" + corsHeaders + "\r\n";
        }
        else if (request.find("GET /api/packets") != std::string::npos) {
            std::lock_guard<std::mutex> lock(packetMutex);
            
            std::stringstream json;
            json << "{\"packets\":[";
            
            size_t count = std::min(capturedPackets.size(), (size_t)100);
            for (size_t i = capturedPackets.size() > 100 ? capturedPackets.size() - 100 : 0; 
                 i < capturedPackets.size(); ++i) {
                if (i > (capturedPackets.size() > 100 ? capturedPackets.size() - 100 : 0)) 
                    json << ",";
                const auto& p = capturedPackets[i];
                json << "{\"id\":" << (i + 1)
                     << ",\"timestamp\":\"" << p.timestamp << "\""
                     << ",\"source\":\"" << p.source << "\""
                     << ",\"destination\":\"" << p.destination << "\""
                     << ",\"protocol\":\"" << p.protocol << "\""
                     << ",\"length\":" << p.length
                     << ",\"info\":\"" << p.payload << "\"}";
            }
            json << "]}";
            body = json.str();
            
            response = "HTTP/1.1 200 OK\r\n" + corsHeaders +
                       "Content-Type: application/json\r\n"
                       "Content-Length: " + std::to_string(body.length()) + "\r\n\r\n" + body;
        }
        else if (request.find("GET /api/stats") != std::string::npos) {
            std::lock_guard<std::mutex> lock(packetMutex);
            
            std::stringstream json;
            json << "{\"totalPackets\":" << globalStats.totalPackets
                 << ",\"tcpPackets\":" << globalStats.tcpPackets
                 << ",\"udpPackets\":" << globalStats.udpPackets
                 << ",\"icmpPackets\":" << globalStats.icmpPackets
                 << ",\"ethernetPackets\":" << globalStats.ethernetPackets
                 << ",\"errorPackets\":" << globalStats.errorPackets << "}";
            body = json.str();
            
            response = "HTTP/1.1 200 OK\r\n" + corsHeaders +
                       "Content-Type: application/json\r\n"
                       "Content-Length: " + std::to_string(body.length()) + "\r\n\r\n" + body;
        }
        else if (request.find("GET /") != std::string::npos) {
            body = "{\"message\":\"Mini Wireshark API - Real Packet Capture\","
                   "\"endpoints\":[\"/api/packets\",\"/api/stats\"]}";
            response = "HTTP/1.1 200 OK\r\n" + corsHeaders +
                       "Content-Type: application/json\r\n"
                       "Content-Length: " + std::to_string(body.length()) + "\r\n\r\n" + body;
        }
        else {
            body = "{\"error\":\"Not found\"}";
            response = "HTTP/1.1 404 Not Found\r\n" + corsHeaders +
                       "Content-Type: application/json\r\n"
                       "Content-Length: " + std::to_string(body.length()) + "\r\n\r\n" + body;
        }
        
        send(clientFd, response.c_str(), response.length(), 0);
        close(clientFd);
    }
    
    close(serverFd);
}

// Packet capture thread
void capturePackets(PacketSniffer& sniffer) {
    while (running) {
        Packet packet = sniffer.getNextPacket();
        
        if (packet.isValid()) {
            std::lock_guard<std::mutex> lock(packetMutex);
            capturedPackets.push_back(packet);
            
            // Update statistics
            globalStats.totalPackets++;
            if (packet.protocol == "TCP") globalStats.tcpPackets++;
            else if (packet.protocol == "UDP") globalStats.udpPackets++;
            else if (packet.protocol == "ICMP") globalStats.icmpPackets++;
            globalStats.ethernetPackets++;
            
            // Keep only last 1000 packets in memory
            if (capturedPackets.size() > 1000) {
                capturedPackets.erase(capturedPackets.begin());
            }
            
            // Print to console
            std::cout << "[" << globalStats.totalPackets << "] "
                      << packet.timestamp << " | "
                      << packet.source << " -> " << packet.destination << " | "
                      << packet.protocol << " | " << packet.length << " bytes | "
                      << packet.payload << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    // Set up signal handler for graceful shutdown
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    // Initialize logger
    Logger::getInstance().setLogFile("packet_analyzer.log");
    Logger::getInstance().setLogLevel(LogLevel::INFO);
    
    std::cout << "============================================" << std::endl;
    std::cout << "    Mini Wireshark - Real Packet Capture    " << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << std::endl;
    
    LOG_INFO("Starting Network Packet Analyzer");
    
    // Get interface from command line or use default
    std::string interface = "";
    if (argc > 1) {
        interface = argv[1];
    }
    
    // Create packet sniffer
    PacketSniffer sniffer(interface);
    
    // Start packet capture
    if (!sniffer.startCapture()) {
        std::cerr << "\nFailed to start packet capture!" << std::endl;
        std::cerr << "Make sure to run with sudo: sudo ./packet_analyzer" << std::endl;
        return 1;
    }
    
    // Start HTTP server in separate thread
    std::thread httpThread(runHttpServer, 8080);
    
    // Start packet capture thread
    std::thread captureThread(capturePackets, std::ref(sniffer));
    
    std::cout << "\nCapturing real network packets..." << std::endl;
    std::cout << "Frontend: http://localhost:3000" << std::endl;
    std::cout << "API: http://localhost:8080/api/packets" << std::endl;
    std::cout << "\nPress Ctrl+C to stop.\n" << std::endl;
    
    // Wait for threads
    captureThread.join();
    httpThread.join();
    
    // Stop capture
    sniffer.stopCapture();
    
    std::cout << "\n=== Final Statistics ===" << std::endl;
    std::cout << "Total Packets: " << globalStats.totalPackets << std::endl;
    std::cout << "TCP: " << globalStats.tcpPackets << std::endl;
    std::cout << "UDP: " << globalStats.udpPackets << std::endl;
    std::cout << "ICMP: " << globalStats.icmpPackets << std::endl;
    
    LOG_INFO("Network Packet Analyzer stopped");
    
    return 0;
}