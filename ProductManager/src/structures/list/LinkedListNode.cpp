#include "LinkedListNode.h"

LinkedListNode::LinkedListNode(const Product& product) {
    this->data = new Product(product);
    this->next = nullptr;
}

LinkedListNode::~LinkedListNode() {
    delete this->data;
}

Product* LinkedListNode::getData(){
    return this->data;
}

LinkedListNode* LinkedListNode::getNext() {
    return this->next;
}

void LinkedListNode::setNext(LinkedListNode* next) {
    this->next = next;
}