#ifndef CSVMANAGER_H
#define CSVMANAGER_H

#include <string>
#include <vector>
#include "Product.h"
#include "CSVException.h"
#include <regex>
#include <fstream>
#include <sstream>
#include <cctype>
#include "ProductException.h"
#include "Logger.h"

class CSVManager
{
private:

    Logger logger;

public: 

    CSVManager();
    ~CSVManager();
    std::vector<Product> readCSV(const std::string& path);
    Logger& getLogger();

private:

    bool fileExists(const std::string& path);
    bool malformedLine(const std::string& line);
    bool barcodeExists(const std::string& barcode, const std::vector<Product>& products);

};

#endif // CSVMANAGER_H