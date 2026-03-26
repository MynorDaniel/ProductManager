#ifndef PRODUCT_EXCEPTION_H
#define PRODUCT_EXCEPTION_H

#include <stdexcept>

class ProductException : public std::exception {
public:
    explicit ProductException(const std::string& message);
    const char* what() const noexcept override;

private:
    std::string message;
};

#endif // PRODUCT_EXCEPTION_H