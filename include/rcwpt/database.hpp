#pragma once

#include "rcwpt/common.hpp"

namespace rcwpt::database {

bool load_database(const std::string& db_directory = "database");
std::vector<ModelInfo> get_all_models();
std::optional<ModelInfo> find_model_by_name(const std::string& name_or_alias);
std::vector<ModelInfo> filter_models_by_series(const std::string& series);
size_t get_total_model_count();

} // namespace rcwpt::database
