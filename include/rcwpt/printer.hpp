#pragma once

#include "rcwpt/common.hpp"

namespace rcwpt::printer {

// Detection
std::vector<PrinterDevice> detect_all_epson_printers();
std::optional<PrinterDevice> detect_primary_epson_printer();

// Spooler management
bool is_admin();
bool restart_print_spooler(bool silent = false);
bool clear_print_queue();
bool send_raw_test_page(const std::string& printer_name);

// Driver & Port inspection
bool is_epson_driver_installed(const std::string& model_keyword = "");
bool set_default_printer(const std::string& printer_name);

} // namespace rcwpt::printer
