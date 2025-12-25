#include "decode/ethernet.h"
#include <iostream>
#include <cstring>

EthernetHeader::EthernetHeader(const u_char* data) {
    std::memcpy(destination, data, 6);
    std::memcpy(source, data + 6, 6);
    ethertype = ntohs(*(reinterpret_cast<const uint16_t*>(data + 12)));
}

void EthernetHeader::printHeader() const {
    std::cout << "Destination: ";
    for (int i = 0; i < 6; ++i) {
        std::cout << std::hex << static_cast<int>(destination[i]);
        if (i < 5) std::cout << ":";
    }
    std::cout << std::dec << "\nSource: ";
    for (int i = 0; i < 6; ++i) {
        std::cout << std::hex << static_cast<int>(source[i]);
        if (i < 5) std::cout << ":";
    }
    std::cout << std::dec << "\nEthertype: " << std::hex << ethertype << std::dec << std::endl;
}