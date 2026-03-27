#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

class Logger {
public:

void addLog(const std::string& log);
std::string saveLogs(const std::string& filename);
void clearLogs();
std::string getLogs() const;

private:

std::string logs;

};

#endif // LOGGER_H