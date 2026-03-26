#include "Product.h"
#include "exceptions/ProductException.h"

void Product::validate() {
    if (name.empty()) {
        throw ProductException("El nombre del producto no puede estar vacio.");
    }
    if (barcode.empty()) {
        throw ProductException("El código de barras del producto no puede estar vacio.");
    }
    if (price < 0) {
        throw ProductException("El precio del producto no puede ser negativo.");
    }
    if (stock < 0) {
        throw ProductException("El stock del producto no puede ser negativo.");
    }
}