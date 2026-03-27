#include "CSVManager.h"

namespace {
const std::regex pattern(
    R"csv(^\s*"([^"]*)"\s*,\s*"([^"]*)"\s*,\s*"([^"]*)"\s*,\s*"([^"]*)"\s*,\s*"([^"]*)"\s*,\s*"([^"]*)"\s*,\s*"([^"]*)"\s*$)csv");

bool isBlankLine(const std::string& value)
{
    for (unsigned char ch : value) {
        if (!std::isspace(ch)) {
            return false;
        }
    }
    return true;
}
}

CSVManager::CSVManager()
{
    logger = Logger();
}

CSVManager::~CSVManager()
{
}

std::vector<Product> CSVManager::readCSV(const std::string& path)
{
    std::vector<Product> products;
    logger.clearLogs();

    if(!fileExists(path)) {
        logger.addLog("El archivo no existe: " + path);
        return products;
    }

    std::ifstream file(path);

    std::string line;

    int lineNumber = 0;
    while(std::getline(file, line)){
        lineNumber++;
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (isBlankLine(line)) {
            continue;
        }

        if(malformedLine(line)) {
            logger.addLog("Línea malformada: " + line + " (Línea: " + std::to_string(lineNumber) + ")");
            continue;
        }

        std::smatch match;
        std::regex_match(line, match, pattern);

        if (match.size() != 8) {
            logger.addLog("Línea malformada: " + line + " (Línea: " + std::to_string(lineNumber) + ")");
            continue;
        }

        const std::string name = match[1].str();
        const std::string barcode = match[2].str();
        const std::string category = match[3].str();
        const std::string expiryDate = match[4].str();
        const std::string brand = match[5].str();
        const std::string priceText = match[6].str();
        const std::string stockText = match[7].str();

        if(barcodeExists(barcode, products)) {
            logger.addLog("Código de barras duplicado: " + barcode + " (Línea: " + std::to_string(lineNumber) + ")");
            continue;
        }

        double price = 0.0;
        int stock = 0;
        try {
            price = std::stod(priceText);
            stock = std::stoi(stockText);
        } catch (const std::exception&) {
            logger.addLog("Precio o stock inválido en línea: " + line + " (Línea: " + std::to_string(lineNumber) + ")");
            continue;
        }

        Product product;
        product.name = name;
        product.barcode = barcode;
        product.category = category;
        product.expiry_date = expiryDate;
        product.brand = brand;
        product.price = price;
        product.stock = stock;

        try
        {
            product.validate();
        }
        catch(const ProductException& e)
        {
            logger.addLog(std::string("Producto inválido: ") + e.what() + " Línea: " + line + " (Línea: " + std::to_string(lineNumber) + ")");
            continue;
        }

        products.push_back(product);
    }

    return products;
}

bool CSVManager::fileExists(const std::string& path)
{
    std::ifstream file(path);
    return file.is_open();
}

bool CSVManager::malformedLine(const std::string& line)
{
    return !std::regex_match(line, pattern);
}

bool CSVManager::barcodeExists(const std::string& barcode, const std::vector<Product>& products)
{
    for (const Product& product : products) {
        if (product.barcode == barcode) {
            return true;
        }
    }
    return false;
}

Logger& CSVManager::getLogger()
{
    return logger;
}