#include "LinkedList.h"

LinkedList::LinkedList()
{
    head = nullptr;
}

LinkedList::~LinkedList()
{
    LinkedListNode* current = head;
    while (current != nullptr)
    {
        LinkedListNode* temp = current;
        current = current->getNext();
        delete temp;
    }
}

void LinkedList::remove(const Product& product)
{
    if (product.barcode.empty()) {
        return;
    }

    LinkedListNode* current = head;
    LinkedListNode* previous = nullptr;

    while (current != nullptr)
    {
        Product* currentProduct = current->getData();
        if (currentProduct != nullptr && currentProduct->barcode == product.barcode)
        {
            if (previous == nullptr)
            {
                head = current->getNext();
            }
            else
            {
                previous->setNext(current->getNext());
            }
            delete current;
            return;
        }

        previous = current;
        current = current->getNext();
    }
}

Product* LinkedList::search(const std::string& barcode)
{
    LinkedListNode* current = head;
    while (current != nullptr)
    {
        Product* currentProduct = current->getData();
        if (currentProduct != nullptr && currentProduct->barcode == barcode)
        {
            return currentProduct;
        }
        current = current->getNext();
    }

    return nullptr;
}

LinkedListNode* LinkedList::getHead()
{
    return head;
}

LinkedListNode* LinkedList::getHead() const
{
    return head;
}
