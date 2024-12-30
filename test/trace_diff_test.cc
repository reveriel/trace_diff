#include "gtest/gtest.h"
#include "trace_diff.h"
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <filesystem>

namespace TraceDiff {
namespace {

class TraceDiffTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Clean up any existing log files before each test
        std::filesystem::remove("tracediff_p1.log");
        std::filesystem::remove("tracediff_p2.log");
        std::filesystem::remove("tracediff_a.log");
        std::filesystem::remove("tracediff_b.log");
        std::filesystem::remove("tracediff_c.log");
    }

    void TearDown() override {
        // Clean up log files after each test
        std::filesystem::remove("tracediff_p1.log");
        std::filesystem::remove("tracediff_p2.log");
        std::filesystem::remove("tracediff_a.log");
        std::filesystem::remove("tracediff_b.log");
        std::filesystem::remove("tracediff_c.log");
    }

    // Helper function to read entire log file
    std::string ReadLogFile(const std::string& prefix) {
        std::string filename = "tracediff_" + prefix + ".log";
        std::ifstream file(filename);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
};

// Test basic logging functionality
TEST_F(TraceDiffTest, BasicLogging) {
    Logger::set_log_prefix("p1");
    TRACE_DIFF_LOG("test_field", "test_value");
    
    std::string log_content = ReadLogFile("p1");
    EXPECT_EQ(log_content, "1:p1:test_field: test_value\n");
}

// Test counter increment behavior
TEST_F(TraceDiffTest, CounterIncrement) {
    // First cycle
    Logger::set_log_prefix("a");
    TRACE_DIFF_LOG("field1", "value1");
    Logger::set_log_prefix("b");
    TRACE_DIFF_LOG("field2", "value2");
    Logger::set_log_prefix("c");
    TRACE_DIFF_LOG("field3", "value3");
    
    // Second cycle - should increment counter on seeing 'a'
    Logger::set_log_prefix("a");
    TRACE_DIFF_LOG("field4", "value4");
    Logger::set_log_prefix("b");
    TRACE_DIFF_LOG("field5", "value5");
    
    std::string log_a = ReadLogFile("a");
    std::string log_b = ReadLogFile("b");
    
    EXPECT_TRUE(log_a.find("1:a:field1") != std::string::npos);
    EXPECT_TRUE(log_a.find("2:a:field4") != std::string::npos);
    EXPECT_TRUE(log_b.find("1:b:field2") != std::string::npos);
    EXPECT_TRUE(log_b.find("2:b:field5") != std::string::npos);
}

// Test thread safety
TEST_F(TraceDiffTest, ThreadSafety) {
    const int num_threads = 10;
    std::vector<std::thread> threads;
    
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([i]() {
            Logger::set_log_prefix("p1");
            TRACE_DIFF_LOG("thread_id", i);
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::string log_content = ReadLogFile("p1");
    int log_count = 0;
    std::string::size_type pos = 0;
    while ((pos = log_content.find("\n", pos)) != std::string::npos) {
        ++log_count;
        ++pos;
    }
    
    EXPECT_EQ(log_count, num_threads);
}

// Test different data types
TEST_F(TraceDiffTest, DifferentTypes) {
    Logger::set_log_prefix("p1");
    
    TRACE_DIFF_LOG("int_field", 42);
    TRACE_DIFF_LOG("double_field", 3.14159);
    TRACE_DIFF_LOG("string_field", "hello");
    TRACE_DIFF_LOG("bool_field", true);
    
    std::string log_content = ReadLogFile("p1");
    EXPECT_TRUE(log_content.find("int_field: 42") != std::string::npos);
    EXPECT_TRUE(log_content.find("double_field: 3.14159") != std::string::npos);
    EXPECT_TRUE(log_content.find("string_field: hello") != std::string::npos);
    EXPECT_TRUE(log_content.find("bool_field: 1") != std::string::npos);
}

}  // namespace
}  // namespace TraceDiff
