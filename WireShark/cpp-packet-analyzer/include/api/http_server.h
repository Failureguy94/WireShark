#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <thread>
#include "../common/packet.h"

class HTTPServer {
public:
    HTTPServer(int port = 8080);
    ~HTTPServer();
    
    void start();
    void stop();
    
    void setPacketProvider(std::function<std::vector<Packet>()> provider);
    void setStatsProvider(std::function<Statistics()> provider);
    
private:
    int port_;
    bool running_;
    std::unique_ptr<std::thread> serverThread_;
    
    std::function<std::vector<Packet>()> packetProvider_;
    std::function<Statistics()> statsProvider_;
    
    void runServer();
    void handleRequest(int clientSocket);
    std::string handleGetPackets();
    std::string handleGetStats();
    std::string createCORSHeaders();
    std::string createResponse(int statusCode, const std::string& content, const std::string& contentType = "application/json");
};

#endif // HTTP_SERVER_H