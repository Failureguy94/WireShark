#include "api/http_server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>

HTTPServer::HTTPServer(int port) : port_(port), running_(false) {}

HTTPServer::~HTTPServer() {
    stop();
}

void HTTPServer::start() {
    if (running_) return;
    
    running_ = true;
    serverThread_ = std::make_unique<std::thread>(&HTTPServer::runServer, this);
    std::cout << "HTTP Server started on port " << port_ << std::endl;
}

void HTTPServer::stop() {
    running_ = false;
    if (serverThread_ && serverThread_->joinable()) {
        serverThread_->join();
    }
}

void HTTPServer::setPacketProvider(std::function<std::vector<Packet>()> provider) {
    packetProvider_ = provider;
}

void HTTPServer::setStatsProvider(std::function<Statistics()> provider) {
    statsProvider_ = provider;
}

void HTTPServer::runServer() {
    int serverFd, newSocket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket failed" << std::endl;
        return;
    }

    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "Setsockopt failed" << std::endl;
        return;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return;
    }

    if (listen(serverFd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return;
    }

    while (running_) {
        if ((newSocket = accept(serverFd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            if (running_) {
                std::cerr << "Accept failed" << std::endl;
            }
            continue;
        }

        handleRequest(newSocket);
        close(newSocket);
    }

    close(serverFd);
}

void HTTPServer::handleRequest(int clientSocket) {
    char buffer[1024] = {0};
    read(clientSocket, buffer, 1024);
    
    std::string request(buffer);
    std::string response;
    
    if (request.find("GET /api/packets") != std::string::npos) {
        response = handleGetPackets();
    }
    else if (request.find("GET /api/stats") != std::string::npos) {
        response = handleGetStats();
    }
    else {
        response = createResponse(404, "{\"error\": \"Not found\"}");
    }
    
    send(clientSocket, response.c_str(), response.length(), 0);
}

std::string HTTPServer::handleGetPackets() {
    if (!packetProvider_) {
        return createResponse(500, "{\"error\": \"No packet provider set\"}");
    }
    
    auto packets = packetProvider_();
    std::stringstream json;
    
    json << "{\"packets\": [";
    for (size_t i = 0; i < packets.size(); ++i) {
        if (i > 0) json << ",";
        json << "{"
             << "\"id\": " << i + 1 << ","
             << "\"timestamp\": \"" << packets[i].timestamp << "\","
             << "\"source\": \"" << packets[i].source << "\","
             << "\"destination\": \"" << packets[i].destination << "\","
             << "\"protocol\": \"" << packets[i].protocol << "\","
             << "\"length\": " << packets[i].length << ","
             << "\"info\": \"" << packets[i].payload << "\""
             << "}";
    }
    json << "]}";
    
    return createResponse(200, json.str());
}

std::string HTTPServer::handleGetStats() {
    if (!statsProvider_) {
        return createResponse(500, "{\"error\": \"No stats provider set\"}");
    }
    
    auto stats = statsProvider_();
    std::stringstream json;
    
    json << "{"
         << "\"totalPackets\": " << stats.totalPackets << ","
         << "\"tcpPackets\": " << stats.tcpPackets << ","
         << "\"udpPackets\": " << stats.udpPackets << ","
         << "\"icmpPackets\": " << stats.icmpPackets << ","
         << "\"ethernetPackets\": " << stats.ethernetPackets << ","
         << "\"errorPackets\": " << stats.errorPackets
         << "}";
    
    return createResponse(200, json.str());
}

std::string HTTPServer::createCORSHeaders() {
    return "Access-Control-Allow-Origin: *\r\n"
           "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
           "Access-Control-Allow-Headers: Content-Type\r\n";
}

std::string HTTPServer::createResponse(int statusCode, const std::string& content, const std::string& contentType) {
    std::string statusText;
    switch (statusCode) {
        case 200: statusText = "OK"; break;
        case 404: statusText = "Not Found"; break;
        case 500: statusText = "Internal Server Error"; break;
        default: statusText = "Unknown"; break;
    }
    
    std::stringstream response;
    response << "HTTP/1.1 " << statusCode << " " << statusText << "\r\n"
             << "Content-Type: " << contentType << "\r\n"
             << createCORSHeaders()
             << "Content-Length: " << content.length() << "\r\n"
             << "\r\n"
             << content;
    
    return response.str();
}