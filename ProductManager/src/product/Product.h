#ifndef PRODUCT_H
#define PRODUCT_H 

#include <string>

struct Product
{
    std::string name;
    std::string barcode;
    std::string category;
    std::string expiry_date;
    std::string brand;
    double price;
    int stock;

    void validate();
};

#endif // PRODUCT_H
