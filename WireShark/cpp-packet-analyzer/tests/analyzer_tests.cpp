#include <gtest/gtest.h>
#include "analyzer.h"

class AnalyzerTests : public ::testing::Test {
protected:
    Analyzer* analyzer;

    void SetUp() override {
        analyzer = new Analyzer();
    }

    void TearDown() override {
        delete analyzer;
    }
};

TEST_F(AnalyzerTests, TestPacketAnalysis) {
    // Assuming we have a method to add packets for analysis
    Packet packet; // Create a mock packet
    analyzer->addPacket(packet);
    
    // Analyze the packet
    analyzer->analyze();

    // Check expected results
    EXPECT_EQ(analyzer->getPacketCount(), 1);
    EXPECT_TRUE(analyzer->hasErrors());
}

TEST_F(AnalyzerTests, TestStatisticsGeneration) {
    // Simulate adding multiple packets
    for (int i = 0; i < 10; ++i) {
        Packet packet; // Create a mock packet
        analyzer->addPacket(packet);
    }

    analyzer->analyze();
    auto stats = analyzer->generateStatistics();

    // Check expected statistics
    EXPECT_EQ(stats.totalPackets, 10);
    EXPECT_EQ(stats.errorPackets, 0);
}

TEST_F(AnalyzerTests, TestResetFunctionality) {
    Packet packet; // Create a mock packet
    analyzer->addPacket(packet);
    analyzer->analyze();

    EXPECT_EQ(analyzer->getPacketCount(), 1);

    analyzer->reset();

    EXPECT_EQ(analyzer->getPacketCount(), 0);
}