#include "Logger.h"

void Logger::addLog(const std::string& log) {
    logs += log + "\n";
}

void Logger::saveLogs(const std::string& filename) {
    std::ofstream file(filename);
    file << logs;
    file.close();
}