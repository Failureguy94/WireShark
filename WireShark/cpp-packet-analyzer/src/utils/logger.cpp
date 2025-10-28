#include "logger.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

Logger::Logger(const std::string& filename) : logFile(filename, std::ios::app) {
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(const std::string& message) {
    if (logFile.is_open()) {
        std::time_t now = std::time(nullptr);
        logFile << std::ctime(&now) << ": " << message << std::endl;
    }
}

void Logger::logError(const std::string& errorMessage) {
    log("ERROR: " + errorMessage);
}

void Logger::logInfo(const std::string& infoMessage) {
    log("INFO: " + infoMessage);
}