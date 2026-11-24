#pragma once

#include "rcwpt/common.hpp"

namespace rcwpt::usb {

struct UsbPrinterInterface {
    std::string device_path;
    std::string friendly_name;
    std::string instance_id;
    std::string ieee1284_device_id;
    uint16_t vid{0x04B8};
    uint16_t pid{0};
};

std::vector<UsbPrinterInterface> enumerate_usb_printers();
std::string query_ieee1284_device_id(const std::string& device_path);
std::string parse_model_from_device_id(const std::string& device_id);

} // namespace rcwpt::usb
