#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include "product/Product.h"

class DataStructure
{
public:
    DataStructure();
    virtual ~DataStructure();

    virtual void insert(const Product& product);
    virtual void remove(const Product& product);
    virtual Product search(const std::string& param);

private:
};

#endif // DATASTRUCTURE_H