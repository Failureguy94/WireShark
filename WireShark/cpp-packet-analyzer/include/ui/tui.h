#ifndef TUI_H
#define TUI_H

#include <string>
#include <vector>

class TUI {
public:
    TUI();
    void displayPacketInfo(const std::vector<std::string>& packetData);
    void displayStatistics(const std::string& stats);
    void clearScreen();
    void update();
    ~TUI();
};

#endif // TUI_H