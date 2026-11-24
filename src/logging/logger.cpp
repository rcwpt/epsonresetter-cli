#include "rcwpt/logger.hpp"
#include <fstream>
#include <mutex>
#include <iomanip>

namespace rcwpt::logging {

static bool g_json_output = false;
static std::string g_log_file = "rcwpt.log";
static std::mutex g_log_mutex;

void init_logger(const std::string& log_file_path) {
    std::lock_guard<std::mutex> lock(g_log_mutex);
    g_log_file = log_file_path;
}

void set_json_output(bool enabled) {
    g_json_output = enabled;
}

bool is_json_output() {
    return g_json_output;
}

void log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(g_log_mutex);
    
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    
    std::ofstream out(g_log_file, std::ios::app);
    if (out.is_open()) {
        const char* lvl_str = "INFO";
        switch (level) {
            case LogLevel::Debug: lvl_str = "DEBUG"; break;
            case LogLevel::Info: lvl_str = "INFO"; break;
            case LogLevel::Warning: lvl_str = "WARN"; break;
            case LogLevel::Error: lvl_str = "ERROR"; break;
            case LogLevel::Success: lvl_str = "SUCCESS"; break;
        }
        out << "[" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S") << "] ["
            << lvl_str << "] " << message << "\n";
    }
}

} // namespace rcwpt::logging
