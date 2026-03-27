#ifndef CSVEXCEPTION_H
#define CSVEXCEPTION_H

#include <exception>
#include <string>

class CSVException : public std::exception
{
private:
    std::string message;
public:
    CSVException(const std::string& msg);
    const char* what() const noexcept override;
};

#endif // CSVEXCEPTION_H