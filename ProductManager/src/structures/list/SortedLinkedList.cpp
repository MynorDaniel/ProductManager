#include "SortedLinkedList.h"

SortedLinkedList::SortedLinkedList()
    : LinkedList()
{}

SortedLinkedList::~SortedLinkedList()
    = default;

void SortedLinkedList::insert(const Product& product)
{
    LinkedListNode* newNode = new LinkedListNode(product);

    if (head == nullptr) {
        head = newNode;
        return;
    }

    Product* headProduct = head->getData();
    if (headProduct == nullptr || headProduct->barcode >= product.barcode) {
        newNode->setNext(head);
        head = newNode;
        return;
    }

    LinkedListNode* current = head;
    while (current->getNext() != nullptr) {
        Product* nextProduct = current->getNext()->getData();
        if (nextProduct == nullptr || nextProduct->barcode >= product.barcode) {
            break;
        }
        current = current->getNext();
    }

    newNode->setNext(current->getNext());
    current->setNext(newNode);
}

std::string SortedLinkedList::toDot() const
{
    return buildDot("SortedLinkedList", "Lista ordenada");
}
