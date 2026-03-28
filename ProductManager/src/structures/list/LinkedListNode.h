#ifndef LINKEDLISTNODE_H
#define LINKEDLISTNODE_H

#include "Product.h"

class LinkedListNode {

private:

    Product* data;
    LinkedListNode* next;

    public:

    LinkedListNode(const Product& product);
    ~LinkedListNode();
    Product* getData();
    LinkedListNode* getNext();
    void setNext(LinkedListNode* next);

};

#endif // LINKEDLISTNODE_H