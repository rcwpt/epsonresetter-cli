#include "rcwpt/printer.hpp"
#include <winspool.h>
#include <shellapi.h>
#include <iostream>

namespace rcwpt::printer {

bool is_admin() {
    BOOL isAdmin = FALSE;
    PSID adminGroup = nullptr;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
                                 DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
        CheckTokenMembership(nullptr, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    return isAdmin == TRUE;
}

bool restart_print_spooler(bool silent) {
    if (!silent) {
        std::cout << "[*] Stopping Print Spooler (spoolsv)...\n";
    }
    system("net stop spooler /y >nul 2>&1");
    Sleep(500);

    if (!silent) {
        std::cout << "[*] Purging stuck spooler print files...\n";
    }
    clear_print_queue();

    if (!silent) {
        std::cout << "[*] Starting Print Spooler service...\n";
    }
    int res = system("net start spooler >nul 2>&1");
    Sleep(300);

    return res == 0;
}

bool clear_print_queue() {
    char windir[MAX_PATH];
    if (GetWindowsDirectoryA(windir, MAX_PATH)) {
        std::string spool_path = std::string(windir) + "\\System32\\spool\\PRINTERS\\*";
        std::string cmd = "del /Q /F \"" + spool_path + "\" >nul 2>&1";
        system(cmd.c_str());
        return true;
    }
    return false;
}

bool send_raw_test_page(const std::string& printer_name) {
    HANDLE hPrinter = nullptr;
    std::wstring wname(printer_name.begin(), printer_name.end());
    if (!OpenPrinterW(const_cast<LPWSTR>(wname.c_str()), &hPrinter, nullptr)) {
        return false;
    }

    DOC_INFO_1W docInfo{};
    docInfo.pDocName = const_cast<LPWSTR>(L"RCWPT Raw Diagnostic Test Page");
    docInfo.pOutputFile = nullptr;
    docInfo.pDatatype = const_cast<LPWSTR>(L"RAW");

    if (StartDocPrinterW(hPrinter, 1, reinterpret_cast<LPBYTE>(&docInfo)) > 0) {
        if (StartPagePrinter(hPrinter)) {
            // Standard ESC/P2 carriage return and test banner
            std::string test_data = "\x1b@\x1b(G\x01\x00\x01\r\n"
                                    "=====================================================\r\n"
                                    "   RCWPT EPSON RESETTER CLI - TEST PRINT PAGE       \r\n"
                                    "=====================================================\r\n"
                                    "Printer: " + printer_name + "\r\n"
                                    "Status : Spooler Communication Verified OK\r\n"
                                    "Engine : Native Win32 Print Spooler API\r\n"
                                    "Date   : PASS\r\n"
                                    "=====================================================\r\n\x0c";
            DWORD written = 0;
            WritePrinter(hPrinter, const_cast<char*>(test_data.data()), (DWORD)test_data.size(), &written);
            EndPagePrinter(hPrinter);
        }
        EndDocPrinter(hPrinter);
    }
    ClosePrinter(hPrinter);
    return true;
}

bool set_default_printer(const std::string& printer_name) {
    std::wstring wname(printer_name.begin(), printer_name.end());
    return SetDefaultPrinterW(wname.c_str()) == TRUE;
}

} // namespace rcwpt::printer
