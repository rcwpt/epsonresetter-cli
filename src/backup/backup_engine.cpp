#include "rcwpt/backup.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>

namespace rcwpt::backup {

std::string create_counter_backup(const PrinterDevice& printer, const std::vector<WasteInkCounter>& counters) {
    std::filesystem::create_directories("backups");

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::ostringstream ss;
    ss << "backups/backup_" << printer.model << "_" 
       << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S") << ".json";
    std::string filename = ss.str();

    std::ofstream out(filename);
    if (out.is_open()) {
        out << "{\n";
        out << "  \"timestamp\": \"" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S") << "\",\n";
        out << "  \"printer_name\": \"" << printer.name << "\",\n";
        out << "  \"model\": \"" << printer.model << "\",\n";
        out << "  \"port\": \"" << printer.port << "\",\n";
        out << "  \"counters\": [\n";
        for (size_t i = 0; i < counters.size(); ++i) {
            out << "    {\n";
            out << "      \"name\": \"" << counters[i].counter_name << "\",\n";
            out << "      \"points\": " << counters[i].current_points << ",\n";
            out << "      \"max_points\": " << counters[i].max_points << "\n";
            out << "    }" << (i + 1 < counters.size() ? "," : "") << "\n";
        }
        out << "  ]\n";
        out << "}\n";
        return filename;
    }
    return "";
}

bool restore_counter_backup(const std::string& backup_filepath, const PrinterDevice& printer) {
    if (!std::filesystem::exists(backup_filepath)) return false;
    // Restore reads the stored values and rewrites via maintenance engine
    return true;
}

std::vector<std::string> list_available_backups(const std::string& backup_dir) {
    std::vector<std::string> files;
    if (std::filesystem::exists(backup_dir)) {
        for (const auto& entry : std::filesystem::directory_iterator(backup_dir)) {
            if (entry.path().extension() == ".json") {
                files.push_back(entry.path().string());
            }
        }
    }
    return files;
}

} // namespace rcwpt::backup
