#include "rcwpt/common.hpp"
#include "rcwpt/terminal.hpp"
#include "rcwpt/printer.hpp"
#include "rcwpt/counter.hpp"
#include "rcwpt/database.hpp"
#include "rcwpt/maintenance.hpp"
#include "rcwpt/security.hpp"
#include "rcwpt/backup.hpp"
#include "rcwpt/logger.hpp"
#include <iostream>
#include <iomanip>

namespace rcwpt::cli {

void print_help() {
    std::cout << APP_NAME << " - Command Line Interface (CLI)\n"
              << "Usage: rcwpt [options]\n\n"
              << "General Commands:\n"
              << "  --help              Display this help manual\n"
              << "  --version           Show application version and build metadata\n"
              << "  --system            Display RCWPT Neofetch-style system dashboard\n"
              << "  --interactive       Launch the interactive terminal menu\n"
              << "  --json              Format output as structured JSON\n\n"
              << "Printer Discovery & Status:\n"
              << "  --detect            Auto-detect connected Epson printers and USB ports\n"
              << "  --list              List all registered print spooler queues\n"
              << "  --model <name>      Target a specific Epson printer model manually\n"
              << "  --status            Inspect active printer online/offline connection state\n"
              << "  --driver-check      Verify Windows spooler driver installation status\n"
              << "  --spooler-status    Inspect and repair Windows Print Spooler service\n\n"
              << "Diagnostics & Maintenance:\n"
              << "  --counters          Read and display waste ink pad counter percentages\n"
              << "  --diagnose          Run comprehensive diagnostic hardware check\n"
              << "  --test-print        Send raw diagnostic pattern page to printer\n"
              << "  --maintenance       Execute waste ink pad counter reset (dry-run by default)\n"
              << "  --force-reset       Execute LIVE waste ink pad reset (requires confirmation)\n"
              << "  --backup            Create an EEPROM counter backup snapshot\n"
              << "  --restore <file>    Restore counter data from backup file\n\n"
              << "Database & Security:\n"
              << "  --models            List all supported printer models in database\n"
              << "  --security-check    Execute Microsoft Defender and supply chain security audit\n"
              << "  --update            Check for model database updates from official repository\n\n"
              << "Examples:\n"
              << "  rcwpt --detect\n"
              << "  rcwpt --counters\n"
              << "  rcwpt --maintenance --model L3250\n"
              << "  rcwpt --security-check\n";
}

int handle_cli(int argc, char* argv[]) {
    terminal::enable_virtual_terminal();

    if (argc <= 1) {
        // Default behavior: show Neofetch dashboard then open interactive menu
        terminal::display_neofetch_dashboard();
        std::cout << "\nLaunching interactive menu (Press any key to proceed or run 'rcwpt --help' for CLI mode)...\n";
        Sleep(1000);
        terminal::run_interactive_menu();
        return 0;
    }

    std::string arg1 = argv[1];

    if (arg1 == "--help" || arg1 == "-h" || arg1 == "/?") {
        print_help();
        return 0;
    }

    if (arg1 == "--version" || arg1 == "-v") {
        std::cout << APP_NAME << " v" << APP_VERSION << " (" << APP_CODENAME << ")\n"
                  << "Engine: Native C++20 / Win32 API\n"
                  << "Author: " << APP_AUTHOR << "\n"
                  << "Repository: " << GITHUB_REPO << "\n";
        return 0;
    }

    if (arg1 == "--system") {
        terminal::display_neofetch_dashboard(printer::detect_primary_epson_printer());
        return 0;
    }

    if (arg1 == "--interactive" || arg1 == "-i") {
        terminal::run_interactive_menu();
        return 0;
    }

    if (arg1 == "--detect") {
        terminal::header("Epson Printer Auto-Detection");
        auto printers = printer::detect_all_epson_printers();
        if (printers.empty()) {
            terminal::warning("No active Epson printer detected over USB or Print Spooler.");
            std::cout << "Manufacturer : EPSON\nModel        : UNKNOWN MODEL\nConnection   : Not Connected\nStatus       : Standby\n";
        } else {
            for (const auto& p : printers) {
                std::cout << "Manufacturer : EPSON\n"
                          << "Model        : " << p.model << "\n"
                          << "Connection   : " << p.connection_type << "\n"
                          << "Port         : " << p.port << "\n"
                          << "Driver       : " << (p.driver.empty() ? "Generic USB" : p.driver) << "\n"
                          << "Status       : " << (p.is_online ? "Ready" : "Offline") << "\n\n";
            }
        }
        return 0;
    }

    if (arg1 == "--counters") {
        terminal::header("Waste Ink Pad Counter Inspection");
        auto printer = printer::detect_primary_epson_printer();
        if (!printer) {
            terminal::error("No Epson printer detected. Please connect printer via USB.");
            return 1;
        }
        auto res = counter::read_waste_ink_counters(*printer);
        if (res.success) {
            std::cout << "Printer Model : " << printer->model << " (" << printer->port << ")\n\n";
            for (const auto& c : res.counters) {
                std::cout << "Counter Name  : " << c.counter_name << "\n"
                          << "Current Value : " << c.current_points << " / " << c.max_points << " points\n"
                          << "Saturation    : " << std::fixed << std::setprecision(1) << c.percentage << "%\n"
                          << "Status        : " << (c.is_overflow ? "OVERFLOW (SERVICE REQUIRED)" : "NORMAL") << "\n\n";
            }
        } else {
            terminal::error(res.error_message);
            return 1;
        }
        return 0;
    }

    if (arg1 == "--models") {
        terminal::header("Epson Model Compatibility Database");
        auto models = database::get_all_models();
        std::cout << std::left << std::setw(10) << "MODEL"
                  << std::setw(24) << "SERIES"
                  << std::setw(16) << "SUPPORT LEVEL"
                  << "STATUS\n";
        std::cout << std::string(75, '-') << "\n";
        for (const auto& m : models) {
            std::string lvl_str = (m.support_level == SupportLevel::Verified) ? "VERIFIED" :
                                  (m.support_level == SupportLevel::Community) ? "COMMUNITY" : "EXPERIMENTAL";
            std::cout << std::left << std::setw(10) << m.id
                      << std::setw(24) << m.series
                      << std::setw(16) << lvl_str
                      << m.status << "\n";
        }
        std::cout << std::string(75, '-') << "\n";
        std::cout << "Total Supported Models: " << models.size() << "\n";
        return 0;
    }

    if (arg1 == "--security-check") {
        terminal::header("RCWPT Defensive Security & Malware Audit");
        auto sec = security::perform_self_security_audit();
        terminal::success("Defender Status: " + sec.defender_status);
        std::cout << "\nSecurity Invariants Verified:\n";
        for (const auto& f : sec.findings) {
            std::cout << "  [OK] " << f << "\n";
        }
        return 0;
    }

    if (arg1 == "--spooler-status") {
        terminal::header("Windows Print Spooler Diagnostics");
        if (printer::is_admin()) {
            terminal::info("Administrator privileges verified.");
            if (printer::restart_print_spooler()) {
                terminal::success("Print Spooler service refreshed and port locks cleared.");
            }
        } else {
            terminal::warning("Run as Administrator to perform Print Spooler service restarts.");
        }
        return 0;
    }

    if (arg1 == "--test-print") {
        terminal::header("Diagnostic Test Page");
        auto printer = printer::detect_primary_epson_printer();
        if (!printer) {
            terminal::error("No printer connected.");
            return 1;
        }
        terminal::info("Sending test page to: " + printer->name);
        if (printer::send_raw_test_page(printer->name)) {
            terminal::success("Test print job submitted to Windows spooler queue.");
        } else {
            terminal::error("Failed to submit test page.");
            return 1;
        }
        return 0;
    }

    if (arg1 == "--maintenance") {
        terminal::header("Waste Ink Pad Maintenance (Dry-Run Safety Mode)");
        auto printer = printer::detect_primary_epson_printer();
        if (!printer) {
            terminal::error("No Epson printer connected.");
            return 1;
        }
        maintenance::ResetExecutionOptions opt;
        opt.dry_run = true; // Safe dry-run by default
        auto res = maintenance::execute_waste_pad_reset(*printer, opt);
        if (res.success) {
            terminal::success(res.message);
            terminal::info("To perform actual write to printer EEPROM, use: rcwpt --force-reset");
        } else {
            terminal::error(res.message);
        }
        return 0;
    }

    if (arg1 == "--force-reset") {
        terminal::header("LIVE Waste Ink Pad Counter Reset");
        terminal::warning("CAUTION: This will write to printer EEPROM memory.");
        terminal::warning("Ensure waste ink pads have been physically inspected or replaced.");
        std::cout << "Type 'YES' to proceed with reset: ";
        std::string confirm;
        std::cin >> confirm;
        if (confirm != "YES") {
            terminal::info("Operation cancelled by user.");
            return 0;
        }
        auto printer = printer::detect_primary_epson_printer();
        if (!printer) {
            terminal::error("No Epson printer connected.");
            return 1;
        }
        maintenance::ResetExecutionOptions opt;
        opt.dry_run = false;
        auto res = maintenance::execute_waste_pad_reset(*printer, opt);
        if (res.success) {
            terminal::success(res.message);
        } else {
            terminal::error(res.message);
        }
        return 0;
    }

    std::cout << "Unknown option: " << arg1 << "\nRun 'rcwpt --help' for available commands.\n";
    return 1;
}

} // namespace rcwpt::cli
