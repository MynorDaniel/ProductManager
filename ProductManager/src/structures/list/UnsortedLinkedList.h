#ifndef UNSORTEDLINKEDLIST_H
#define UNSORTEDLINKEDLIST_H

#include "LinkedList.h"

class UnsortedLinkedList : public LinkedList
{
public:
    UnsortedLinkedList();
    ~UnsortedLinkedList() override;

    void insert(const Product& product) override;
    std::string toDot() const;
};

#endif // UNSORTEDLINKEDLIST_H