#include "ProductException.h"

ProductException::ProductException(const std::string& message) : message(message) {}

const char* ProductException::what() const noexcept {
    return message.c_str();
}