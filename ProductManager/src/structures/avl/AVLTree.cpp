#include "AVLTree.h"

#include <map>
#include <sstream>

namespace
{
    void clearSubtree(AVLNode *node)
    {
        if (node == nullptr)
        {
            return;
        }

        clearSubtree(node->getLeft());
        clearSubtree(node->getRight());
        delete node;
    }

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

    std::string productLabel(const Product *product)
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

    int appendNodeDot(const AVLNode *node, std::ostringstream &dot, std::map<const AVLNode *, int> &ids, int &nextId)
    {
        if (node == nullptr)
        {
            return -1;
        }

        std::map<const AVLNode *, int>::const_iterator it = ids.find(node);
        if (it != ids.end())
        {
            return it->second;
        }

        int id = nextId++;
        ids[node] = id;

        dot << "  n" << id << " [label=\"" << productLabel(node->getData()) << "\"];\n";

        int leftId = appendNodeDot(node->getLeft(), dot, ids, nextId);
        int rightId = appendNodeDot(node->getRight(), dot, ids, nextId);

        if (leftId >= 0)
        {
            dot << "  n" << id << " -> n" << leftId << " [label=\"L\"];\n";
        }
        if (rightId >= 0)
        {
            dot << "  n" << id << " -> n" << rightId << " [label=\"R\"];\n";
        }

        return id;
    }
}

AVLTree::AVLTree()
{
    root = nullptr;
}

AVLTree::~AVLTree()
{
    clearSubtree(root);
    root = nullptr;
}

AVLNode *AVLTree::getRoot() const
{
    return root;
}

void AVLTree::insert(const Product &product)
{
    bool grew = false;
    root = insert(root, &product, grew);
}

void AVLTree::remove(const Product &product)
{
    bool decreased = false;
    root = remove(root, &product, decreased);
}

Product *AVLTree::search(const std::string &name)
{
    return search(root, name);
}

Product *AVLTree::search(AVLNode *node, const std::string &name)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    Product *currentProduct = node->getData();
    if (currentProduct != nullptr && currentProduct->name == name)
    {
        return currentProduct;
    }

    if (name < currentProduct->name)
    {
        return search(node->getLeft(), name);
    }
    else
    {
        return search(node->getRight(), name);
    }
}

std::string AVLTree::inOrder() const
{
    if (root == nullptr)
    {
        return "\nLista de productos en AVL:\n";
    }

    return "\nLista de productos en AVL:\n" + inOrder(root);
}

std::string AVLTree::inOrder(const AVLNode *node) const
{
    if (node == nullptr)
    {
        return "";
    }

    std::string result;
    result += inOrder(node->getLeft());

    Product *product = node->getData();
    if (product != nullptr)
    {
        result += product->name + ", " + product->barcode + ", " + product->category + ", " + product->expiry_date + ", " + product->brand + ", " + std::to_string(product->price) + ", " + std::to_string(product->stock) + "\n";
    }

    result += inOrder(node->getRight());
    return result;
}

std::string AVLTree::toDot() const
{
    std::ostringstream dot;
    dot << "digraph AVLTree {\n";
    dot << "  rankdir=TB;\n";
    dot << "  node [shape=box, fontsize=10];\n";

    if (root == nullptr)
    {
        dot << "  empty [label=\"AVL vacio\"];\n";
        dot << "}\n";
        return dot.str();
    }

    std::map<const AVLNode *, int> ids;
    int nextId = 0;
    appendNodeDot(root, dot, ids, nextId);

    dot << "}\n";
    return dot.str();
}

AVLNode *AVLTree::rotateRR(AVLNode *n, AVLNode *n1)
{
    n->setRight(n1->getLeft());
    n1->setLeft(n);
    if (n1->getFe() == +1)
    {
        n->setFe(0);
        n1->setFe(0);
    }
    else
    {
        n->setFe(+1);
        n1->setFe(-1);
    }
    return n1;
}

AVLNode *AVLTree::rotateLL(AVLNode *n, AVLNode *n1)
{
    n->setLeft(n1->getRight());
    n1->setRight(n);
    if (n1->getFe() == -1)
    {
        n->setFe(0);
        n1->setFe(0);
    }
    else
    {
        n->setFe(-1);
        n1->setFe(1);
    }
    return n1;
}

AVLNode *AVLTree::rotateRL(AVLNode *n, AVLNode *n1)
{
    AVLNode *n2;
    n2 = n1->getLeft();
    n->setRight(n2->getLeft());
    n2->setLeft(n);
    n1->setLeft(n2->getRight());
    n2->setRight(n1);
    if (n2->getFe() == +1)
    {
        n->setFe(-1);
    }
    else
    {
        n->setFe(0);
    }
    if (n2->getFe() == -1)
    {
        n1->setFe(+1);
    }
    else
    {
        n1->setFe(0);
    }
    n2->setFe(0);
    return n2;
}

AVLNode *AVLTree::rotateLR(AVLNode *n, AVLNode *n1)
{
    AVLNode *n2;
    n2 = n1->getRight();
    n->setLeft(n2->getRight());
    n2->setRight(n);
    n1->setRight(n2->getLeft());
    n2->setLeft(n1);
    if (n2->getFe() == +1)
    {
        n1->setFe(-1);
    }
    else
    {
        n1->setFe(0);
    }
    if (n2->getFe() == -1)
    {
        n->setFe(1);
    }
    else
    {
        n->setFe(0);
    }
    n2->setFe(0);
    return n2;
}

AVLNode *AVLTree::insert(AVLNode *node, const Product *product, bool &grew)
{
    AVLNode *n1;
    if (node == nullptr)
    {
        node = new AVLNode(product);
        grew = true;
    }
    else if (product->name < node->getData()->name)
    {
        AVLNode *leftN;
        leftN = insert(node->getLeft(), product, grew);
        node->setLeft(leftN);
        if (grew)
        {
            // decrementa el fe por aumentar la altura de rama izquierda
            switch (node->getFe())
            {
            case 1:
                // tenía +1 y creció su izquierda
                node->setFe(0);
                grew = false;
                break;
            case 0:
                // tenía 0 y creció su izquierda
                node->setFe(-1); // árbol sigue creciendo
                break;
            case -1:
                // aplicar rotación a la izquierda
                n1 = node->getLeft();
                if (n1->getFe() == -1)
                    node = rotateLL(node, n1);
                else
                    node = rotateLR(node, n1);
                grew = false; // árbol deja de crecer en ambas rotaciones
            }
        }
    }
    else if (product->name > node->getData()->name)
    {
        AVLNode *rightN;
        rightN = insert(node->getRight(), product, grew);
        node->setRight(rightN);
        if (grew)
        // siempre se comprueba si creció en altura
        {
            // incrementa el fe por aumentar la altura de rama derecha
            switch (node->getFe())
            {
            case -1:
                // tenía -1 y creció su derecha
                node->setFe(0);
                grew = false; // árbol deja de crecer
                break;
            case 0:
                // tenía 0 y creció su derecha
                node->setFe(1); // árbol sigue creciendo
                break;
            case 1:
                // aplicar rotación a la derecha
                n1 = node->getRight();
                if (n1->getFe() == +1)
                    node = rotateRR(node, n1);
                else
                    node = rotateRL(node, n1);
                grew = false; // árbol deja de crecer en ambas rotaciones
            }
        }
    }
    else
        throw ProductException("No puede haber claves repetidas ");
    return node;
}

AVLNode *AVLTree::remove(AVLNode *node, const Product *product, bool &decreased)
{
    if (node == nullptr)
    {
        decreased = false;
        return nullptr;
    }

    const std::string &key = product->name;
    Product *nodeProduct = node->getData();

    if (key < nodeProduct->name)
    {
        node->setLeft(remove(node->getLeft(), product, decreased));

        if (decreased)
        {
            switch (node->getFe())
            {
            case -1:
                node->setFe(0);
                decreased = true;
                break;
            case 0:
                node->setFe(1);
                decreased = false;
                break;
            case 1:
            {
                AVLNode *rightNode = node->getRight();
                if (rightNode == nullptr)
                {
                    node->setFe(0);
                    decreased = true;
                    break;
                }

                if (rightNode->getFe() >= 0)
                {
                    node = rotateRR(node, rightNode);
                    decreased = (rightNode->getFe() != -1);
                }
                else
                {
                    node = rotateRL(node, rightNode);
                    decreased = true;
                }
                break;
            }
            }
        }

        return node;
    }

    if (key > nodeProduct->name)
    {
        node->setRight(remove(node->getRight(), product, decreased));

        if (decreased)
        {
            switch (node->getFe())
            {
            case 1:
                node->setFe(0);
                decreased = true;
                break;
            case 0:
                node->setFe(-1);
                decreased = false;
                break;
            case -1:
            {
                AVLNode *leftNode = node->getLeft();
                if (leftNode == nullptr)
                {
                    node->setFe(0);
                    decreased = true;
                    break;
                }

                if (leftNode->getFe() <= 0)
                {
                    node = rotateLL(node, leftNode);
                    decreased = (leftNode->getFe() != 1);
                }
                else
                {
                    node = rotateLR(node, leftNode);
                    decreased = true;
                }
                break;
            }
            }
        }

        return node;
    }

    // Nodo encontrado: caso sin hijos o con un solo hijo.
    if (node->getLeft() == nullptr)
    {
        AVLNode *rightChild = node->getRight();
        node->setRight(nullptr);
        delete node;
        decreased = true;
        return rightChild;
    }

    if (node->getRight() == nullptr)
    {
        AVLNode *leftChild = node->getLeft();
        node->setLeft(nullptr);
        delete node;
        decreased = true;
        return leftChild;
    }

    // Dos hijos: intercambiar con el predecesor (máximo del subárbol izquierdo)
    AVLNode *predecessor = node->getLeft();
    while (predecessor->getRight() != nullptr)
    {
        predecessor = predecessor->getRight();
    }

    Product *currentData = node->getData();
    node->setData(predecessor->getData());
    predecessor->setData(currentData);

    node->setLeft(remove(node->getLeft(), currentData, decreased));

    if (decreased)
    {
        switch (node->getFe())
        {
        case -1:
            node->setFe(0);
            decreased = true;
            break;
        case 0:
            node->setFe(1);
            decreased = false;
            break;
        case 1:
        {
            AVLNode *rightNode = node->getRight();
            if (rightNode == nullptr)
            {
                node->setFe(0);
                decreased = true;
                break;
            }

            if (rightNode->getFe() >= 0)
            {
                node = rotateRR(node, rightNode);
                decreased = (rightNode->getFe() != -1);
            }
            else
            {
                node = rotateRL(node, rightNode);
                decreased = true;
            }
            break;
        }
        }
    }

    return node;
}

























