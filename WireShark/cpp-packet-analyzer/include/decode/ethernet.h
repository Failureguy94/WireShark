#ifndef ETHERNET_H
#define ETHERNET_H

#include <cstdint>

class EthernetHeader {
public:
    EthernetHeader(const uint8_t* data);
    void decode();
    void print() const;

private:
    uint8_t destinationMAC[6];
    uint8_t sourceMAC[6];
    uint16_t etherType;
};

#endif // ETHERNET_H