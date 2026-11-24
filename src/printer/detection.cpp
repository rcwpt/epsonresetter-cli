#include "rcwpt/printer.hpp"
#include "rcwpt/usb.hpp"
#include <winspool.h>
#include <algorithm>
#include <sstream>

namespace rcwpt::printer {

static std::string wide_to_narrow(const std::wstring& wstr) {
    if (wstr.empty()) return {};
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    std::string str(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), str.data(), size, nullptr, nullptr);
    return str;
}

std::vector<PrinterDevice> detect_all_epson_printers() {
    std::vector<PrinterDevice> results;

    // 1. Query Windows Print Spooler (EnumPrinters)
    DWORD flags = PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS;
    DWORD bytesNeeded = 0;
    DWORD printerCount = 0;

    EnumPrintersW(flags, nullptr, 2, nullptr, 0, &bytesNeeded, &printerCount);
    if (bytesNeeded > 0) {
        std::vector<BYTE> buffer(bytesNeeded);
        if (EnumPrintersW(flags, nullptr, 2, buffer.data(), bytesNeeded, &bytesNeeded, &printerCount)) {
            auto* pinfo = reinterpret_cast<PRINTER_INFO_2W*>(buffer.data());
            for (DWORD i = 0; i < printerCount; ++i) {
                std::string name = pinfo[i].pPrinterName ? wide_to_narrow(pinfo[i].pPrinterName) : "";
                std::string driver = pinfo[i].pDriverName ? wide_to_narrow(pinfo[i].pDriverName) : "";
                std::string port = pinfo[i].pPortName ? wide_to_narrow(pinfo[i].pPortName) : "";

                std::string lower_name = name;
                std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);
                std::string lower_driver = driver;
                std::transform(lower_driver.begin(), lower_driver.end(), lower_driver.begin(), ::tolower);

                if (lower_name.find("epson") != std::string::npos || lower_driver.find("epson") != std::string::npos) {
                    PrinterDevice dev;
                    dev.name = name;
                    dev.driver = driver;
                    dev.port = port;
                    dev.connection_type = (port.find("USB") != std::string::npos || port.find("EPUSB") != std::string::npos) ? "USB" : "Network/Spooler";
                    dev.is_online = (pinfo[i].Attributes & PRINTER_ATTRIBUTE_WORK_OFFLINE) == 0;
                    dev.is_default = (pinfo[i].Attributes & PRINTER_ATTRIBUTE_DEFAULT) != 0;
                    
                    // Extract model name from printer name
                    dev.model = name;
                    size_t epson_pos = dev.model.find("EPSON ");
                    if (epson_pos != std::string::npos) {
                        dev.model = dev.model.substr(epson_pos + 6);
                    }
                    size_t series_pos = dev.model.find(" Series");
                    if (series_pos != std::string::npos) {
                        dev.model = dev.model.substr(0, series_pos);
                    }
                    
                    results.push_back(dev);
                }
            }
        }
    }

    // 2. Query USB Plug and Play via SetupAPI
    auto usb_interfaces = usb::enumerate_usb_printers();
    for (const auto& usb_dev : usb_interfaces) {
        bool already_present = false;
        for (auto& r : results) {
            if (r.connection_type == "USB") {
                already_present = true;
                if (!usb_dev.ieee1284_device_id.empty()) {
                    r.device_id = usb_dev.ieee1284_device_id;
                }
                break;
            }
        }
        if (!already_present) {
            PrinterDevice dev;
            dev.name = usb_dev.friendly_name.empty() ? "EPSON USB Printer" : usb_dev.friendly_name;
            dev.model = usb::parse_model_from_device_id(usb_dev.ieee1284_device_id);
            if (dev.model.empty()) dev.model = "USB-Epson";
            dev.port = "USB001";
            dev.driver = "USB Printing Support";
            dev.connection_type = "USB";
            dev.device_id = usb_dev.ieee1284_device_id;
            dev.instance_id = usb_dev.instance_id;
            dev.is_online = true;
            results.push_back(dev);
        }
    }

    return results;
}

std::optional<PrinterDevice> detect_primary_epson_printer() {
    auto printers = detect_all_epson_printers();
    if (printers.empty()) return std::nullopt;

    // Prefer online USB printer, or default printer
    for (const auto& p : printers) {
        if (p.is_default && p.is_online) return p;
    }
    for (const auto& p : printers) {
        if (p.is_online && p.connection_type == "USB") return p;
    }
    return printers.front();
}

bool is_epson_driver_installed(const std::string& model_keyword) {
    DWORD bytesNeeded = 0;
    DWORD driverCount = 0;
    EnumPrinterDriversW(nullptr, nullptr, 2, nullptr, 0, &bytesNeeded, &driverCount);
    if (bytesNeeded > 0) {
        std::vector<BYTE> buffer(bytesNeeded);
        if (EnumPrinterDriversW(nullptr, nullptr, 2, buffer.data(), bytesNeeded, &bytesNeeded, &driverCount)) {
            auto* dinfo = reinterpret_cast<DRIVER_INFO_2W*>(buffer.data());
            for (DWORD i = 0; i < driverCount; ++i) {
                std::string name = dinfo[i].pName ? wide_to_narrow(dinfo[i].pName) : "";
                std::string lower = name;
                std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
                if (lower.find("epson") != std::string::npos) {
                    if (model_keyword.empty() || lower.find(model_keyword) != std::string::npos) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

} // namespace rcwpt::printer
