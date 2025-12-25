cmake_minimum_required(VERSION 3.10)
project(CPPPacketAnalyzer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

# Simplified build - only core files for now
set(SOURCES
    src/main.cpp
    src/capture/pcap_capture.cpp
    src/analysis/analyzer.cpp
    src/utils/logger.cpp
)

add_executable(packet_analyzer ${SOURCES})

find_package(PkgConfig REQUIRED)
pkg_check_modules(PCAP REQUIRED libpcap)

target_link_libraries(packet_analyzer ${PCAP_LIBRARIES} pthread)
target_include_directories(packet_analyzer PRIVATE ${PCAP_INCLUDE_DIRS})