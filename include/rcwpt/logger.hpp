#pragma once

#include "rcwpt/common.hpp"

namespace rcwpt::logging {

void init_logger(const std::string& log_file_path = "rcwpt.log");
void log(LogLevel level, const std::string& message);
void set_json_output(bool enabled);
bool is_json_output();

} // namespace rcwpt::logging
