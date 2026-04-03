#include "LinkedList.h"

#include <sstream>

namespace
{
    std::string escapeDot(const std::string &value)
    {
        std::string escaped;
        escaped.reserve(value.size());

        for (char ch : value)
        {
            if (ch == '\\' || ch == '"')
            {
                escaped.push_back('\\');
            }
            escaped.push_back(ch);
        }

        return escaped;
    }

    std::string productToLabel(const Product *product)
    {
        if (product == nullptr)
        {
            return "Producto nulo";
        }

        std::ostringstream label;
        label << "name: " << escapeDot(product->name)
              << "\\nbarcode: " << escapeDot(product->barcode)
              << "\\ncategory: " << escapeDot(product->category)
              << "\\nexpiry: " << escapeDot(product->expiry_date)
              << "\\nbrand: " << escapeDot(product->brand)
              << "\\nprice: " << product->price
              << "\\nstock: " << product->stock;
        return label.str();
    }
}

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

std::string LinkedList::buildDot(const std::string &graphName, const std::string &title) const
{
    std::ostringstream dot;
    dot << "digraph " << graphName << " {\n";
    dot << "  rankdir=LR;\n";
    dot << "  label=\"" << escapeDot(title) << "\";\n";
    dot << "  node [shape=box, fontsize=10];\n";

    if (head == nullptr)
    {
        dot << "  empty [label=\"Estructura vacia\"];\n";
        dot << "}\n";
        return dot.str();
    }

    int index = 0;
    for (LinkedListNode *current = head; current != nullptr; current = current->getNext())
    {
        dot << "  n" << index << " [label=\"" << productToLabel(current->getData()) << "\"];\n";
        if (current->getNext() != nullptr)
        {
            dot << "  n" << index << " -> n" << (index + 1) << ";\n";
        }
        index++;
    }

    dot << "}\n";
    return dot.str();
}
