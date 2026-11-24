#pragma once

#include "rcwpt/common.hpp"

namespace rcwpt::counter {

struct CounterReadResult {
    bool success{false};
    std::string printer_model;
    std::string port;
    std::vector<WasteInkCounter> counters;
    std::string raw_response;
    std::string error_message;
};

CounterReadResult read_waste_ink_counters(const PrinterDevice& printer);
std::vector<uint8_t> build_bdc_eeprom_read_packet(uint16_t address, uint8_t length);
bool parse_eeprom_response(const std::vector<uint8_t>& response, uint16_t expected_addr, std::vector<uint8_t>& out_bytes);

} // namespace rcwpt::counter
