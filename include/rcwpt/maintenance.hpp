#pragma once

#include "rcwpt/common.hpp"
#include "rcwpt/counter.hpp"

namespace rcwpt::maintenance {

struct ResetExecutionOptions {
    bool dry_run{true};
    bool backup_first{true};
    bool auto_confirm{false};
    std::string custom_backup_path;
};

struct ResetExecutionResult {
    bool success{false};
    bool dry_run{true};
    std::string backup_path;
    std::string message;
    uint32_t before_counter{0};
    uint32_t after_counter{0};
    bool verified{false};
};

ResetExecutionResult execute_waste_pad_reset(
    const PrinterDevice& printer,
    const ResetExecutionOptions& options = ResetExecutionOptions{}
);

} // namespace rcwpt::maintenance
