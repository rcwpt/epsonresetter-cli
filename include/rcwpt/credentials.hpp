#pragma once

#include "rcwpt/common.hpp"

namespace rcwpt::credentials {

// Secure Credential Management via Windows Credential Manager & DPAPI
bool save_credential(const std::string& target_name, const std::string& secret);
std::optional<std::string> get_credential(const std::string& target_name);
bool delete_credential(const std::string& target_name);

// DPAPI encrypted local blob storage
std::vector<uint8_t> protect_data(const std::vector<uint8_t>& plaintext);
std::vector<uint8_t> unprotect_data(const std::vector<uint8_t>& ciphertext);

} // namespace rcwpt::credentials
