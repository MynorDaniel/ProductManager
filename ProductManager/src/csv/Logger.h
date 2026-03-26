#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>

class Logger {
public:

void addLog(const std::string& log);
void saveLogs(const std::string& filename);

private:

std::string logs;

};

#endif // LOGGER_H