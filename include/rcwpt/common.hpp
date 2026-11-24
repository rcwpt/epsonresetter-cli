#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <functional>
#include <chrono>
#include <filesystem>

namespace rcwpt {

constexpr const char* APP_NAME = "RCWPT Epson Resetter CLI Suite";
constexpr const char* APP_VERSION = "1.0.0";
constexpr const char* APP_CODENAME = "Native Sentinel";
constexpr const char* APP_AUTHOR = "rcwpt (@c.wijaya.t)";
constexpr const char* GITHUB_REPO = "https://github.com/rcwpt/epsonresetter-cli";

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Success
};

struct PrinterDevice {
    std::string name;
    std::string model;
    std::string port;
    std::string driver;
    std::string connection_type; // USB, Network, Spooler
    std::string device_id;       // IEEE 1284 Device ID
    std::string instance_id;     // Plug and Play Instance ID
    bool is_online{false};
    bool is_default{false};
};

struct WasteInkCounter {
    std::string counter_name;
    uint32_t current_points{0};
    uint32_t max_points{6207};
    double percentage{0.0};
    bool is_overflow{false};
};

enum class SupportLevel {
    Verified,
    Community,
    Experimental,
    Unsupported
};

struct ModelInfo {
    std::string id;
    std::string name;
    std::string series;
    std::vector<std::string> aliases;
    std::string vid;
    std::string pid;
    std::vector<std::string> counters;
    uint32_t max_counter{6207};
    std::string protocol;
    SupportLevel support_level{SupportLevel::Community};
    std::string status;
    std::string source;
};

} // namespace rcwpt
