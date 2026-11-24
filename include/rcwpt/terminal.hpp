#pragma once

#include "rcwpt/common.hpp"

namespace rcwpt::terminal {

void enable_virtual_terminal();
void clear_screen();
void print_color(const std::string& text, const std::string& ansi_code);

// Color helpers
void info(const std::string& msg);
void success(const std::string& msg);
void warning(const std::string& msg);
void error(const std::string& msg);
void header(const std::string& title);

// Neofetch-style Dashboard
void display_neofetch_dashboard(const std::optional<PrinterDevice>& detected_printer = std::nullopt);

// Interactive CLI Menu
void run_interactive_menu();

} // namespace rcwpt::terminal
