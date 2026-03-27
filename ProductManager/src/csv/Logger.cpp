#include "Logger.h"

void Logger::addLog(const std::string& log) {
    logs += log + "\n";
}

std::string Logger::saveLogs(const std::string& filename) {

    std::filesystem::path filePath(filename);
    std::filesystem::path absolutePath = std::filesystem::absolute(filename);

    std::ofstream file(filename);
    file << logs;
    file.close();

    return absolutePath.string();
}

void Logger::clearLogs() {
    logs.clear();
}

std::string Logger::getLogs() const {
    if(logs.empty()) {
        return "Carga exitosa.";
    }
    return logs;
}