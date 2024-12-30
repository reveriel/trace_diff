#ifndef TRACE_DIFF_H
#define TRACE_DIFF_H

#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <atomic>
#include <set>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>

namespace TraceDiff {

// Forward declarations of container serialization
template<typename T>
std::string serialize_container(const std::vector<T>& vec);

template<typename K, typename V>
std::string serialize_container(const std::map<K,V>& map);

template<typename T>
std::string serialize_container(const std::set<T>& set);

template<typename T>
std::string serialize_container(const std::unordered_set<T>& set);

template<typename K, typename V>
std::string serialize_container(const std::unordered_map<K,V>& map);

// Type traits to detect container types
template<typename T, typename = void>
struct is_container : std::false_type {};

template<typename T>
struct is_container<std::vector<T>> : std::true_type {};

template<typename K, typename V>
struct is_container<std::map<K,V>> : std::true_type {};

template<typename T>
struct is_container<std::set<T>> : std::true_type {};

template<typename T>
struct is_container<std::unordered_set<T>> : std::true_type {};

template<typename K, typename V>
struct is_container<std::unordered_map<K,V>> : std::true_type {};


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
        ss << counter_ << ":" << log_prefix_ << ":" << field_name << ": ";
        
        // Use container serialization if it's a container type
        if constexpr (is_container<T>::value) {
            ss << serialize_container(value);
        } else {
            ss << value;
        }
        ss << "\n";
        
        log_file_ << ss.str();
        log_file_.flush();
    }
};

// Container serialization implementations
template<typename T>
std::string serialize_container(const std::vector<T>& vec) {
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << vec[i];
    }
    ss << "]";
    return ss.str();
}

template<typename K, typename V>
std::string serialize_container(const std::map<K,V>& map) {
    std::stringstream ss;
    ss << "{";
    bool first = true;
    for (const auto& [key, value] : map) {
        if (!first) ss << ", ";
        ss << key << ": " << value;
        first = false;
    }
    ss << "}";
    return ss.str();
}

template<typename T>
std::string serialize_container(const std::set<T>& set) {
    std::stringstream ss;
    ss << "[";
    bool first = true;
    for (const auto& value : set) {
        if (!first) ss << ", ";
        ss << value;
        first = false;
    }
    ss << "]";
    return ss.str();
}

template<typename T>
std::string serialize_container(const std::unordered_set<T>& set) {
    // covnert to std::set
    std::set<T> ordered_set(set.begin(), set.end());
    return serialize_container(ordered_set);
}

template<typename K, typename V>
std::string serialize_container(const std::unordered_map<K,V>& map) {
    // covnert to std::map
    std::map<K,V> ordered_map(map.begin(), map.end());
    return serialize_container(ordered_map);
}

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
