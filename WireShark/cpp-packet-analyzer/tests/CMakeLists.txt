cmake_minimum_required(VERSION 3.10)

# Set the project name
project(PacketAnalyzerTests)

# Find the required packages
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add the test executable
add_executable(analyzer_tests analyzer_tests.cpp)

# Link the test executable with GTest and the main application
target_link_libraries(analyzer_tests ${GTEST_LIBRARIES} pthread)