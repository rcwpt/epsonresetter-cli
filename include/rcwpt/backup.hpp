#pragma once

#include "rcwpt/common.hpp"
#include "rcwpt/counter.hpp"

namespace rcwpt::backup {

struct BackupHeader {
    std::string timestamp;
    std::string printer_model;
    std::string serial_number;
    std::string port;
    std::vector<WasteInkCounter> counters;
};

std::string create_counter_backup(const PrinterDevice& printer, const std::vector<WasteInkCounter>& counters);
bool restore_counter_backup(const std::string& backup_filepath, const PrinterDevice& printer);
std::vector<std::string> list_available_backups(const std::string& backup_dir = "backups");

} // namespace rcwpt::backup
