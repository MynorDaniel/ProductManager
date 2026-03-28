#include "UnsortedLinkedList.h"

UnsortedLinkedList::UnsortedLinkedList()
    : LinkedList()
{}

UnsortedLinkedList::~UnsortedLinkedList()
    = default;

void UnsortedLinkedList::insert(const Product& product)
{
    LinkedListNode* newNode = new LinkedListNode(product);
    newNode->setNext(head);
    head = newNode;
}