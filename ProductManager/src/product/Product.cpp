#include "Product.h"
#include "exceptions/ProductException.h"
#include <regex>

void Product::validate() {
    static const std::regex isoDatePattern(R"(^\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])$)");

    if (name.empty()) {
        throw ProductException("El nombre del producto no puede estar vacio.");
    }
    if (barcode.empty()) {
        throw ProductException("El código de barras del producto no puede estar vacio.");
    }
    if (brand.empty()) {
        throw ProductException("La marca del producto no puede estar vacia.");
    }
    if (!std::regex_match(expiry_date, isoDatePattern)) {
        throw ProductException("La fecha de caducidad debe estar en formato ISO (YYYY-MM-DD).");
    }
    if (price < 0) {
        throw ProductException("El precio del producto no puede ser negativo.");
    }
    if (stock < 0) {
        throw ProductException("El stock del producto no puede ser negativo.");
    }
}