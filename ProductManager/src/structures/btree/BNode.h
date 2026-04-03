#ifndef BNODE_H
#define BNODE_H

#include "Product.h"

class BNode
{
private:
    Product **keys;
    BNode **children;
    int count;
    int m;

public:
    BNode(int order);
    ~BNode();

    bool isFull();
    bool isSemiEmpty();
    Product* getKey(int index);
    Product* getKey(int index) const;
    void setKey(int index, Product* product);
    BNode* getChild(int index);
    BNode* getChild(int index) const;
    void setChild(int index, BNode* child);
    int getCount();
    int getCount() const;
    void setCount(int newCount);
};
#endif // BNODE_H