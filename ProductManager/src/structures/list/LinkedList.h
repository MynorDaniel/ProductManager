#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "LinkedListNode.h"
#include "DataStructure.h"

class LinkedList : public DataStructure
{
protected:
    LinkedListNode* head;

public:
    LinkedList();
    ~LinkedList() override;

    void remove(const Product& product) override;
    Product* search(const std::string& param) override;
    LinkedListNode* getHead();
    LinkedListNode* getHead() const;
};

#endif // LINKEDLIST_H
