#include "rcwpt/usb.hpp"
#include <setupapi.h>
#include <initguid.h>
#include <devpkey.h>
#include <algorithm>
#include <regex>

// GUID_DEVINTERFACE_USBPRINT: 28d78f35-d727-11d0-8e49-00a0c91bc942
DEFINE_GUID(GUID_DEVINTERFACE_USBPRINT, 
    0x28d78f35, 0xd727, 0x11d0, 0x8e, 0x49, 0x00, 0xa0, 0xc9, 0x1b, 0xc9, 0x42);

namespace rcwpt::usb {

std::vector<UsbPrinterInterface> enumerate_usb_printers() {
    std::vector<UsbPrinterInterface> devices;

    HDEVINFO hDevInfo = SetupDiGetClassDevsA(
        &GUID_DEVINTERFACE_USBPRINT,
        nullptr,
        nullptr,
        DIGCF_PRESENT | DIGCF_DEVICEINTERFACE
    );

    if (hDevInfo == INVALID_HANDLE_VALUE) {
        return devices;
    }

    SP_DEVICE_INTERFACE_DATA ifData{};
    ifData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    DWORD index = 0;
    while (SetupDiEnumDeviceInterfaces(hDevInfo, nullptr, &GUID_DEVINTERFACE_USBPRINT, index++, &ifData)) {
        DWORD requiredSize = 0;
        SetupDiGetDeviceInterfaceDetailA(hDevInfo, &ifData, nullptr, 0, &requiredSize, nullptr);

        if (requiredSize > 0) {
            std::vector<BYTE> buffer(requiredSize);
            auto* pDetail = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA_A>(buffer.data());
            pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);

            SP_DEVINFO_DATA devInfoData{};
            devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

            if (SetupDiGetDeviceInterfaceDetailA(hDevInfo, &ifData, pDetail, requiredSize, nullptr, &devInfoData)) {
                std::string path = pDetail->DevicePath;
                UsbPrinterInterface dev;
                dev.device_path = path;

                // Query friendly name or description
                char friendlyName[256] = {0};
                if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME,
                                                     nullptr, reinterpret_cast<PBYTE>(friendlyName), sizeof(friendlyName), nullptr) ||
                    SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devInfoData, SPDRP_DEVICEDESC,
                                                     nullptr, reinterpret_cast<PBYTE>(friendlyName), sizeof(friendlyName), nullptr)) {
                    dev.friendly_name = friendlyName;
                }

                // Query hardware ID / Instance ID
                char instanceId[256] = {0};
                if (SetupDiGetDeviceInstanceIdA(hDevInfo, &devInfoData, instanceId, sizeof(instanceId), nullptr)) {
                    dev.instance_id = instanceId;
                    
                    // Parse VID and PID
                    std::string id_str = instanceId;
                    std::transform(id_str.begin(), id_str.end(), id_str.begin(), ::toupper);
                    if (id_str.find("VID_04B8") != std::string::npos) {
                        dev.vid = 0x04B8; // Seiko Epson Corp
                        size_t pid_pos = id_str.find("PID_");
                        if (pid_pos != std::string::npos && pid_pos + 8 <= id_str.size()) {
                            dev.pid = static_cast<uint16_t>(std::stoul(id_str.substr(pid_pos + 4, 4), nullptr, 16));
                        }
                    }
                }

                dev.ieee1284_device_id = query_ieee1284_device_id(path);
                devices.push_back(dev);
            }
        }
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);
    return devices;
}

std::string query_ieee1284_device_id(const std::string& device_path) {
    HANDLE hDevice = CreateFileA(
        device_path.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
    );

    if (hDevice == INVALID_HANDLE_VALUE) {
        return "";
    }

    // IOCTL_USBPRINT_GET_1284_ID = 0x00220014
    constexpr DWORD IOCTL_USBPRINT_GET_1284_ID = 0x00220014;
    char buffer[1024] = {0};
    DWORD bytesReturned = 0;

    if (DeviceIoControl(hDevice, IOCTL_USBPRINT_GET_1284_ID, nullptr, 0, buffer, sizeof(buffer) - 1, &bytesReturned, nullptr)) {
        CloseHandle(hDevice);
        // The first 2 bytes can be length header (big-endian) in some USBPRINT implementations
        std::string raw(buffer, bytesReturned);
        size_t mfg_pos = raw.find("MFG:");
        if (mfg_pos != std::string::npos) {
            return raw.substr(mfg_pos);
        }
        return raw;
    }

    CloseHandle(hDevice);
    return "";
}

std::string parse_model_from_device_id(const std::string& device_id) {
    if (device_id.empty()) return "";
    std::regex mdl_regex(R"(MDL:([^;]+))", std::regex::icase);
    std::smatch match;
    if (std::regex_search(device_id, match, mdl_regex)) {
        return match[1].str();
    }
    return "";
}

} // namespace rcwpt::usb
