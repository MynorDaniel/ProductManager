#include "CSVException.h"

CSVException::CSVException(const std::string& msg): message(msg){}

const char* CSVException::what() const noexcept {
    return message.c_str();
}