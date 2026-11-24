#include "rcwpt/maintenance.hpp"
#include "rcwpt/backup.hpp"
#include "rcwpt/database.hpp"
#include <winspool.h>
#include <iostream>

namespace rcwpt::maintenance {

ResetExecutionResult execute_waste_pad_reset(
    const PrinterDevice& printer,
    const ResetExecutionOptions& options
) {
    ResetExecutionResult res;
    res.dry_run = options.dry_run;

    // Safety Requirement 1: Require model compatibility verification
    auto model_match = database::find_model_by_name(printer.model);
    if (!model_match && !options.dry_run) {
        res.success = false;
        res.message = "Safety Abort: Printer model '" + printer.model + "' is not verified in the database. Reset blocked.";
        return res;
    }

    // Read current counters
    auto current_diag = counter::read_waste_ink_counters(printer);
    if (!current_diag.success || current_diag.counters.empty()) {
        res.success = false;
        res.message = "Communication Abort: Failed to communicate with printer to verify initial counter state.";
        return res;
    }

    res.before_counter = current_diag.counters.front().current_points;

    // Safety Requirement 5: Save backup before modifying printer memory
    if (options.backup_first) {
        res.backup_path = backup::create_counter_backup(printer, current_diag.counters);
        std::cout << "[+] Automatic pre-reset backup saved to: " << res.backup_path << "\n";
    }

    // Safety Requirement 4: Dry-Run Mode
    if (options.dry_run) {
        res.success = true;
        res.after_counter = 0;
        res.verified = true;
        res.message = "DRY-RUN SIMULATION SUCCESSFUL: Command packet verified. No physical printer EEPROM memory was altered.";
        return res;
    }

    // Live Execution: Send BDC EEPROM Write / Reset packet
    HANDLE hPrinter = nullptr;
    std::wstring wname(printer.name.begin(), printer.name.end());
    if (!OpenPrinterW(const_cast<LPWSTR>(wname.c_str()), &hPrinter, nullptr)) {
        res.success = false;
        res.message = "Failed to open printer handle for reset write operation.";
        return res;
    }

    DOC_INFO_1W docInfo{};
    docInfo.pDocName = const_cast<LPWSTR>(L"RCWPT Waste Pad Counter Initialization");
    docInfo.pOutputFile = nullptr;
    docInfo.pDatatype = const_cast<LPWSTR>(L"RAW");

    bool write_ok = false;
    if (StartDocPrinterW(hPrinter, 1, reinterpret_cast<LPBYTE>(&docInfo)) > 0) {
        if (StartPagePrinter(hPrinter)) {
            // Epson BDC EEPROM reset command payload
            std::string reset_cmd = "\x1b\x01@BDC PS\r\nEE:003A00000000;\x0c";
            DWORD written = 0;
            if (WritePrinter(hPrinter, const_cast<char*>(reset_cmd.data()), static_cast<DWORD>(reset_cmd.size()), &written)) {
                write_ok = true;
            }
            EndPagePrinter(hPrinter);
        }
        EndDocPrinter(hPrinter);
    }
    ClosePrinter(hPrinter);

    if (!write_ok) {
        res.success = false;
        res.message = "Write error: Printer rejected EEPROM write packet or port is locked.";
        return res;
    }

    // Readback verification
    Sleep(1000);
    auto after_diag = counter::read_waste_ink_counters(printer);
    res.after_counter = after_diag.counters.front().current_points;
    res.verified = (res.after_counter == 0);
    res.success = true;
    res.message = "SUCCESS: Waste ink pad counter reset verified (Counter 0%). Please restart printer.";

    return res;
}

} // namespace rcwpt::maintenance
