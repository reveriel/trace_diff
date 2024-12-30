#ifndef TRACE_DIFF_H
#define TRACE_DIFF_H

#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <atomic>
#include <set>

namespace TraceDiff {

class Logger {
private:
    static std::atomic<uint64_t> counter_;
    static std::string log_prefix_;
    static std::mutex mutex_;
    static std::ofstream log_file_;
    static bool initialized_;
    static std::set<std::string> seen_prefixes_;  // Track seen prefixes

    static void init() {
        // Not needed anymore
    }

    static void increment_counter() {
        ++counter_;
    }

public:
    static void set_log_prefix(const std::string& prefix) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        // Close existing log file if initialized
        if (initialized_) {
            log_file_.close();
            initialized_ = false;
        }
        
        // If we see a prefix that starts a new cycle (e.g., going back to first prefix)
        if (!seen_prefixes_.empty() && seen_prefixes_.find(prefix) != seen_prefixes_.end()) {
            increment_counter();
            seen_prefixes_.clear();
        }
        
        seen_prefixes_.insert(prefix);
        log_prefix_ = prefix;
        
        // Initialize new log file
        std::string filename = "tracediff_" + log_prefix_ + ".log";
        log_file_.open(filename, std::ios::out | std::ios::app);  // Open in append mode
        initialized_ = true;
    }

    template<typename T>
    static void log(const std::string& field_name, const T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!initialized_) {
            // If somehow log() is called before set_log_prefix(), initialize with current prefix
            std::string filename = "tracediff_" + log_prefix_ + ".log";
            log_file_.open(filename, std::ios::out | std::ios::app);
            initialized_ = true;
        }
        
        std::stringstream ss;
        ss << counter_ << ":" << log_prefix_ << ":" << field_name << ": " << value << "\n";
        
        log_file_ << ss.str();
        log_file_.flush();
    }
};

// Static member initialization
std::atomic<uint64_t> Logger::counter_{1};
std::string Logger::log_prefix_;
std::mutex Logger::mutex_;
std::ofstream Logger::log_file_;
bool Logger::initialized_ = false;
std::set<std::string> Logger::seen_prefixes_;

} // namespace TraceDiff

#ifndef ENABLE_TRACE_DIFF_LOG
#define ENABLE_TRACE_DIFF_LOG
#endif

#ifdef ENABLE_TRACE_DIFF_LOG
    #define TRACE_DIFF_LOG(field_name, value) \
        TraceDiff::Logger::log(field_name, value)
#else
    #define TRACE_DIFF_LOG(field_name, value) ((void)0)
#endif

#endif // TRACE_DIFF_H
