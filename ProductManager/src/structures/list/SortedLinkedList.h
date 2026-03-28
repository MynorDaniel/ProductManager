#ifndef SORTEDLINKEDLIST_H
#define SORTEDLINKEDLIST_H

#include "LinkedList.h"

class SortedLinkedList : public LinkedList
{
public:
    SortedLinkedList();
    ~SortedLinkedList() override;

    void insert(const Product& product) override;
};

#endif // SORTEDLINKEDLIST_H
