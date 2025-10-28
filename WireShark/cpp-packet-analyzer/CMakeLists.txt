project(CPPPacketAnalyzer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

file(GLOB SOURCES
    src/main.cpp
    src/capture/pcap_capture.cpp
    src/decode/ethernet.cpp
    src/decode/ip.cpp
    src/decode/tcp.cpp
    src/decode/udp.cpp
    src/analysis/analyzer.cpp
    src/ui/tui.cpp
    src/utils/logger.cpp
)

add_executable(packet_analyzer ${SOURCES})

find_package(PkgConfig REQUIRED)
pkg_check_modules(PCAP REQUIRED libpcap)

target_link_libraries(packet_analyzer ${PCAP_LIBRARIES})
target_include_directories(packet_analyzer PRIVATE ${PCAP_INCLUDE_DIRS})