#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include "product/Product.h"

class DataStructure
{
public:
    DataStructure() = default;
    virtual ~DataStructure() = 0;

    virtual void insert(const Product& product) = 0;
    virtual void remove(const Product& product) = 0;
    virtual Product* search(const std::string& param) = 0;

private:
};

inline DataStructure::~DataStructure() = default;

#endif // DATASTRUCTURE_H