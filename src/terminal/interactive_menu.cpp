#include "rcwpt/terminal.hpp"
#include "rcwpt/printer.hpp"
#include "rcwpt/counter.hpp"
#include "rcwpt/database.hpp"
#include "rcwpt/maintenance.hpp"
#include "rcwpt/security.hpp"
#include <iostream>
#include <conio.h>

namespace rcwpt::terminal {

void run_interactive_menu() {
    enable_virtual_terminal();
    std::optional<PrinterDevice> active_printer = printer::detect_primary_epson_printer();

    while (true) {
        clear_screen();
        print_color("===================================================\n", "\x1b[1;36m");
        print_color("             RCWPT EPSON RESETTER CLI             \n", "\x1b[1;33m");
        print_color("===================================================\n\n", "\x1b[1;36m");

        std::cout << "  [A] AUTO DETECT EPSON PRINTER\n";
        std::cout << "  [B] SELECT PRINTER MODEL\n";
        std::cout << "  [C] PRINTER DIAGNOSTICS & SPOOLER\n";
        std::cout << "  [D] READ WASTE INK COUNTER\n";
        std::cout << "  [E] MAINTENANCE & RESET OPERATIONS\n";
        std::cout << "  [F] BACKUP / RESTORE\n";
        std::cout << "  [G] DRIVER MANAGEMENT\n";
        std::cout << "  [H] PRINT TEST PAGE\n";
        std::cout << "  [I] MODEL DATABASE BROWSER\n";
        std::cout << "  [J] SECURITY AUDIT\n";
        std::cout << "  [K] UPDATE DATABASE\n";
        std::cout << "  [L] SYSTEM INFORMATION (NEOFETCH)\n";
        std::cout << "  [Q] EXIT\n\n";

        print_color("===================================================\n", "\x1b[90m");
        if (active_printer) {
            std::cout << "Detected Printer: " << active_printer->name << "\n";
            std::cout << "Connection      : " << active_printer->connection_type << " (" << active_printer->port << ")\n";
            std::cout << "Status          : " << (active_printer->is_online ? "READY / ONLINE" : "OFFLINE") << "\n";
        } else {
            std::cout << "Detected Printer: None (Plug in USB or install driver)\n";
            std::cout << "Connection      : Standby\n";
            std::cout << "Status          : Waiting for printer connection\n";
        }
        print_color("===================================================\n\n", "\x1b[90m");

        std::cout << "Select option: ";
        char ch = _getch();
        ch = toupper(ch);
        std::cout << ch << "\n\n";

        if (ch == 'Q' || ch == 27) { // Q or ESC
            clear_screen();
            success("Thank you for using RCWPT Epson Resetter CLI Suite.");
            info("Developed by RCWPT (@c.wijaya.t)");
            break;
        }

        switch (ch) {
            case 'A': {
                header("Auto-Detecting Epson Printers");
                auto printers = printer::detect_all_epson_printers();
                if (printers.empty()) {
                    warning("No connected Epson printer found. Ensure printer is turned ON and USB cable is connected.");
                } else {
                    success("Found " + std::to_string(printers.size()) + " Epson printer(s):");
                    for (const auto& p : printers) {
                        std::cout << "  - " << p.name << " | Port: " << p.port << " | Driver: " << p.driver << "\n";
                    }
                    active_printer = printers.front();
                }
                break;
            }
            case 'B': {
                header("Select Printer Model Manually");
                std::cout << "Enter Epson model keyword (e.g. L3250, L3110, L120): ";
                std::string kw;
                std::cin >> kw;
                auto model = database::find_model_by_name(kw);
                if (model) {
                    success("Model matched: " + model->name + " (" + model->series + ")");
                    PrinterDevice manual_p;
                    manual_p.name = model->name;
                    manual_p.model = model->id;
                    manual_p.port = "USB001";
                    manual_p.connection_type = "USB";
                    manual_p.is_online = true;
                    active_printer = manual_p;
                } else {
                    warning("Model keyword '" + kw + "' not found in database.");
                }
                break;
            }
            case 'C': {
                header("Printer Diagnostics & Spooler");
                if (printer::is_admin()) {
                    info("Running Print Spooler repair...");
                    if (printer::restart_print_spooler()) {
                        success("Spooler restarted and locked queues cleared.");
                    }
                } else {
                    warning("Administrator privileges recommended for full spooler service reset.");
                }
                break;
            }
            case 'D': {
                header("Waste Ink Pad Counter Diagnostics");
                if (!active_printer) {
                    warning("No printer selected. Run [A] Auto-Detect first.");
                } else {
                    auto res = counter::read_waste_ink_counters(*active_printer);
                    if (res.success) {
                        for (const auto& c : res.counters) {
                            std::cout << "  " << c.counter_name << ": " << c.current_points << " / " << c.max_points
                                      << " (" << c.percentage << "%)\n";
                        }
                    } else {
                        warning("Could not read counters: " + res.error_message);
                    }
                }
                break;
            }
            case 'E': {
                header("Maintenance & Waste Pad Reset");
                if (!active_printer) {
                    warning("No printer selected. Run [A] Auto-Detect first.");
                } else {
                    maintenance::ResetExecutionOptions opt;
                    opt.dry_run = false;
                    auto res = maintenance::execute_waste_pad_reset(*active_printer, opt);
                    if (res.success) {
                        success(res.message);
                    } else {
                        warning(res.message);
                    }
                }
                break;
            }
            case 'F': {
                header("Backup / Restore Counter Snapshots");
                info("Backups stored in 'backups/' directory.");
                break;
            }
            case 'G': {
                header("Driver Management");
                info("Checking installed Epson print drivers...");
                if (printer::is_epson_driver_installed()) {
                    success("Official Epson driver is installed in Windows.");
                } else {
                    warning("No registered Epson print driver found in Windows spooler.");
                }
                break;
            }
            case 'H': {
                header("Print Test Page");
                if (!active_printer) {
                    warning("No printer selected.");
                } else {
                    info("Sending raw diagnostic test page to " + active_printer->name + "...");
                    printer::send_raw_test_page(active_printer->name);
                }
                break;
            }
            case 'I': {
                header("Epson Model Database");
                auto all = database::get_all_models();
                std::cout << "Database contains " << all.size() << " validated models:\n";
                for (size_t i = 0; i < std::min(all.size(), size_t(15)); ++i) {
                    std::cout << "  [" << (i + 1) << "] " << all[i].name << " (" << all[i].series << ") - " 
                              << all[i].status << "\n";
                }
                if (all.size() > 15) {
                    std::cout << "  ... and " << (all.size() - 15) << " more.\n";
                }
                break;
            }
            case 'J': {
                header("Security & Malware Integrity Audit");
                auto sec = security::perform_self_security_audit();
                if (sec.is_clean) {
                    success("Audit Complete: 100% Clean! 0 Trojans, 0 Droppers, 0 Keygens detected.");
                }
                break;
            }
            case 'L': {
                display_neofetch_dashboard(active_printer);
                break;
            }
            default:
                warning("Invalid option. Please choose from the menu.");
                break;
        }

        std::cout << "\nPress any key to return to menu...";
        _getch();
    }
}

} // namespace rcwpt::terminal
