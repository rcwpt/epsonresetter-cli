#pragma once

#include "rcwpt/common.hpp"

namespace rcwpt::security {

struct SecurityAuditResult {
    bool is_clean{true};
    std::string defender_status;
    std::vector<std::string> audited_binaries;
    std::vector<std::string> quarantined_files;
    std::vector<std::string> findings;
};

SecurityAuditResult perform_self_security_audit();
std::string compute_file_sha256(const std::string& filepath);

} // namespace rcwpt::security
