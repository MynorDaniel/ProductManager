#include "BTree.h"

#include <cctype>
#include <map>
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

    int appendNodeDot(const BNode *node, std::ostringstream &dot, std::map<const BNode *, int> &ids, int &nextId)
    {
        if (node == nullptr)
        {
            return -1;
        }

        std::map<const BNode *, int>::const_iterator it = ids.find(node);
        if (it != ids.end())
        {
            return it->second;
        }

        int id = nextId++;
        ids[node] = id;

        dot << "  n" << id << " [label=\"";
        for (int i = 0; i < node->getCount(); i++)
        {
            if (i > 0)
            {
                dot << "|";
            }
            dot << productLabel(node->getKey(i));
        }
        dot << "\"];\n";

        for (int i = 0; i <= node->getCount(); i++)
        {
            const BNode *child = node->getChild(i);
            if (child != nullptr)
            {
                int childId = appendNodeDot(child, dot, ids, nextId);
                dot << "  n" << id << " -> n" << childId << ";\n";
            }
        }

        return id;
    }

    bool isLeaf(const BNode *node)
    {
        return node == nullptr || node->getChild(0) == nullptr;
    }

    int minKeys(int order)
    {
        return ((order + 1) / 2) - 1;
    }

    int findKeyIndex(const BNode *node, const std::string &expiryDate)
    {
        int idx = 0;
        const int count = node->getCount();

        while (idx < count)
        {
            Product *key = node->getKey(idx);
            if (key == nullptr || key->expiry_date >= expiryDate)
            {
                break;
            }
            idx++;
        }

        return idx;
    }

    Product *rightMostKey(BNode *node)
    {
        BNode *current = node;
        while (current != nullptr && current->getChild(current->getCount()) != nullptr)
        {
            current = current->getChild(current->getCount());
        }
        if (current == nullptr || current->getCount() == 0)
        {
            return nullptr;
        }
        return current->getKey(current->getCount() - 1);
    }

    Product *leftMostKey(BNode *node)
    {
        BNode *current = node;
        while (current != nullptr && current->getChild(0) != nullptr)
        {
            current = current->getChild(0);
        }
        if (current == nullptr || current->getCount() == 0)
        {
            return nullptr;
        }
        return current->getKey(0);
    }

    void removeFromLeaf(BNode *node, int idx)
    {
        int count = node->getCount();
        Product *target = node->getKey(idx);
        delete target;

        for (int i = idx; i < count - 1; i++)
        {
            node->setKey(i, node->getKey(i + 1));
        }

        node->setKey(count - 1, nullptr);
        node->setCount(count - 1);
    }

    void mergeChildren(BNode *parent, int idx)
    {
        BNode *leftChild = parent->getChild(idx);
        BNode *rightChild = parent->getChild(idx + 1);

        int leftCount = leftChild->getCount();
        int rightCount = rightChild->getCount();

        leftChild->setKey(leftCount, parent->getKey(idx));

        for (int i = 0; i < rightCount; i++)
        {
            leftChild->setKey(leftCount + 1 + i, rightChild->getKey(i));
        }

        for (int i = 0; i <= rightCount; i++)
        {
            leftChild->setChild(leftCount + 1 + i, rightChild->getChild(i));
        }

        leftChild->setCount(leftCount + 1 + rightCount);

        int parentCount = parent->getCount();
        for (int i = idx; i < parentCount - 1; i++)
        {
            parent->setKey(i, parent->getKey(i + 1));
            parent->setChild(i + 1, parent->getChild(i + 2));
        }

        parent->setKey(parentCount - 1, nullptr);
        parent->setChild(parentCount, nullptr);
        parent->setCount(parentCount - 1);

        delete rightChild;
    }

    void borrowFromPrev(BNode *parent, int idx)
    {
        BNode *child = parent->getChild(idx);
        BNode *sibling = parent->getChild(idx - 1);

        int childCount = child->getCount();
        for (int i = childCount; i > 0; i--)
        {
            child->setKey(i, child->getKey(i - 1));
        }
        for (int i = childCount + 1; i > 0; i--)
        {
            child->setChild(i, child->getChild(i - 1));
        }

        child->setKey(0, parent->getKey(idx - 1));
        child->setChild(0, sibling->getChild(sibling->getCount()));
        child->setCount(childCount + 1);

        int siblingCount = sibling->getCount();
        parent->setKey(idx - 1, sibling->getKey(siblingCount - 1));
        sibling->setKey(siblingCount - 1, nullptr);
        sibling->setChild(siblingCount, nullptr);
        sibling->setCount(siblingCount - 1);
    }

    void borrowFromNext(BNode *parent, int idx)
    {
        BNode *child = parent->getChild(idx);
        BNode *sibling = parent->getChild(idx + 1);

        int childCount = child->getCount();
        child->setKey(childCount, parent->getKey(idx));
        child->setChild(childCount + 1, sibling->getChild(0));
        child->setCount(childCount + 1);

        parent->setKey(idx, sibling->getKey(0));

        int siblingCount = sibling->getCount();
        for (int i = 0; i < siblingCount - 1; i++)
        {
            sibling->setKey(i, sibling->getKey(i + 1));
        }
        for (int i = 0; i < siblingCount; i++)
        {
            sibling->setChild(i, sibling->getChild(i + 1));
        }

        sibling->setKey(siblingCount - 1, nullptr);
        sibling->setChild(siblingCount, nullptr);
        sibling->setCount(siblingCount - 1);
    }

    void fillChild(BNode *parent, int idx, int minKeyCount)
    {
        BNode *leftSibling = (idx > 0) ? parent->getChild(idx - 1) : nullptr;
        BNode *rightSibling = (idx < parent->getCount()) ? parent->getChild(idx + 1) : nullptr;

        if (leftSibling != nullptr && leftSibling->getCount() > minKeyCount)
        {
            borrowFromPrev(parent, idx);
        }
        else if (rightSibling != nullptr && rightSibling->getCount() > minKeyCount)
        {
            borrowFromNext(parent, idx);
        }
        else
        {
            if (idx < parent->getCount())
            {
                mergeChildren(parent, idx);
            }
            else
            {
                mergeChildren(parent, idx - 1);
            }
        }
    }

    void deleteKey(BNode *node, const std::string &key, int order)
    {
        if (node == nullptr)
        {
            return;
        }

        int idx = findKeyIndex(node, key);
        int count = node->getCount();

        if (idx < count)
        {
            Product *nodeKey = node->getKey(idx);
            if (nodeKey != nullptr && nodeKey->expiry_date == key)
            {
                if (isLeaf(node))
                {
                    removeFromLeaf(node, idx);
                    return;
                }

                int minKeyCount = minKeys(order);
                BNode *leftChild = node->getChild(idx);
                BNode *rightChild = node->getChild(idx + 1);

                if (leftChild != nullptr && leftChild->getCount() > minKeyCount)
                {
                    Product *pred = rightMostKey(leftChild);
                    if (pred != nullptr)
                    {
                        Product replacement = *pred;
                        delete nodeKey;
                        node->setKey(idx, new Product(replacement));
                        deleteKey(leftChild, replacement.expiry_date, order);
                    }
                }
                else if (rightChild != nullptr && rightChild->getCount() > minKeyCount)
                {
                    Product *succ = leftMostKey(rightChild);
                    if (succ != nullptr)
                    {
                        Product replacement = *succ;
                        delete nodeKey;
                        node->setKey(idx, new Product(replacement));
                        deleteKey(rightChild, replacement.expiry_date, order);
                    }
                }
                else
                {
                    mergeChildren(node, idx);
                    deleteKey(node->getChild(idx), key, order);
                }
                return;
            }
        }

        if (isLeaf(node))
        {
            return;
        }

        int minKeyCount = minKeys(order);
        bool wasLastChild = (idx == node->getCount());
        BNode *child = node->getChild(idx);

        if (child != nullptr && child->getCount() == minKeyCount)
        {
            fillChild(node, idx, minKeyCount);
        }

        if (wasLastChild && idx > node->getCount())
        {
            child = node->getChild(idx - 1);
        }
        else
        {
            child = node->getChild(idx);
        }

        deleteKey(child, key, order);
    }

    void clearSubtree(BNode *node)
    {
        if (node == nullptr)
        {
            return;
        }

        const int count = node->getCount();
        for (int i = 0; i <= count; i++)
        {
            clearSubtree(node->getChild(i));
        }

        for (int i = 0; i < count; i++)
        {
            delete node->getKey(i);
            node->setKey(i, nullptr);
        }

        delete node;
    }

    bool isIsoDate(const std::string &date)
    {
        if (date.size() != 10)
        {
            return false;
        }

        for (int i = 0; i < 10; i++)
        {
            if (i == 4 || i == 7)
            {
                if (date[i] != '-')
                {
                    return false;
                }
            }
            else if (!std::isdigit(static_cast<unsigned char>(date[i])))
            {
                return false;
            }
        }

        return true;
    }

    bool parseDateRange(const std::string &dates, std::string &startDate, std::string &endDate)
    {
        size_t separator = dates.find('#');
        if (separator == std::string::npos || separator != dates.rfind('#'))
        {
            return false;
        }

        startDate = dates.substr(0, separator);
        endDate = dates.substr(separator + 1);

        if (startDate.empty() || endDate.empty())
        {
            return false;
        }

        if (!isIsoDate(startDate) || !isIsoDate(endDate))
        {
            return false;
        }

        if (startDate > endDate)
        {
            return false;
        }

        return true;
    }

    Product *findFirstInRange(BNode *node, const std::string &startDate, const std::string &endDate)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        const int count = node->getCount();

        for (int i = 0; i < count; i++)
        {
            Product *fromLeft = findFirstInRange(node->getChild(i), startDate, endDate);
            if (fromLeft != nullptr)
            {
                return fromLeft;
            }

            Product *key = node->getKey(i);
            if (key == nullptr)
            {
                continue;
            }

            if (key->expiry_date >= startDate && key->expiry_date <= endDate)
            {
                return key;
            }

            if (key->expiry_date > endDate)
            {
                return nullptr;
            }
        }

        return findFirstInRange(node->getChild(count), startDate, endDate);
    }
}

BTree::BTree(int order)
{
    m = order;
    root = nullptr;
}

BTree::BTree()
{
    m = 3;
    root = nullptr;
}

BTree::~BTree()
{
    clearSubtree(root);
    root = nullptr;
}

BNode *BTree::getRoot()
{
    return root;
}

void BTree::setRoot(BNode *newRoot)
{
    root = newRoot;
}

void BTree::insert(const Product &product)
{
    Product *newProduct = new Product(product);

    try
    {
        Product *median = nullptr;
        BNode *newRight = nullptr;
        bool grows = push(root, newProduct, median, newRight);

        if (grows)
        {
            BNode *newRoot = new BNode(m);
            newRoot->setCount(1);
            newRoot->setKey(0, median);
            newRoot->setChild(0, root);
            newRoot->setChild(1, newRight);
            root = newRoot;
        }
    }
    catch (...)
    {
        delete newProduct;
        throw;
    }
}

void BTree::remove(const Product &product)
{
    if (root == nullptr)
    {
        return;
    }

    deleteKey(root, product.expiry_date, m);

    if (root->getCount() == 0)
    {
        BNode *oldRoot = root;
        if (root->getChild(0) != nullptr)
        {
            root = root->getChild(0);
        }
        else
        {
            root = nullptr;
        }

        oldRoot->setChild(0, nullptr);
        delete oldRoot;
    }
}

bool BTree::searchNode(BNode *current, Product *product, int &k)
{
    if (current == nullptr || product == nullptr)
    {
        k = 0;
        return false;
    }

    const int count = current->getCount();
    int index = 0;

    // Orden ascendente por fecha
    while (index < count)
    {
        Product *key = current->getKey(index);
        if (key == nullptr)
        {
            break;
        }

        if (product->expiry_date > key->expiry_date)
        {
            index++;
            continue;
        }
        break;
    }

    if (index < count)
    {
        Product *key = current->getKey(index);
        if (key != nullptr && product->expiry_date == key->expiry_date)
        {
            k = index;
            return true;
        }
    }

    k = index;
    return false;
}

BNode *BTree::search(Product* product, int &n)
{
    return search(root, product, n);
}

BNode *BTree::search(BNode *current, Product* product, int &n)
{
    if (current == nullptr)
    {
        return nullptr;
    }

    bool found = searchNode(current, product, n);
    if (found)
    {
        return current;
    }

    return search(current->getChild(n), product, n);
}

Product *BTree::search(const std::string &dates)
{
    std::string startDate;
    std::string endDate;

    if (!parseDateRange(dates, startDate, endDate))
    {
        return nullptr;
    }

    return findFirstInRange(root, startDate, endDate);
}

std::string BTree::listAscending() const
{
    std::string output = "Lista creciente de claves en árbol B:\n";
    inOrder(root, output);
    return output;
}

void BTree::inOrder(const BNode *node, std::string &output) const
{
    if (node == nullptr)
    {
        return;
    }

    inOrder(node->getChild(0), output);

    const int count = node->getCount();
    for (int k = 0; k < count; k++)
    {
        Product *key = node->getKey(k);
        if (key != nullptr)
        {
            output += key->expiry_date + " | " + key->name + " | " + key->barcode + "\n";
        }
        inOrder(node->getChild(k + 1), output);
    }
}

bool BTree::push(BNode *current, Product *product, Product *&median, BNode *&newRight)
{
    int k = 0;
    bool goesUp = false;

    if (current == nullptr)
    {
        goesUp = true;
        median = product;
        newRight = nullptr;
    }
    else
    {
        bool exists = searchNode(current, product, k);
        if (exists)
        {
            throw ProductException("No puede haber claves repetidas");
        }

        goesUp = push(current->getChild(k), product, median, newRight);
        if (goesUp)
        {
            if (current->isFull())
            {
                splitNode(current, median, newRight, k);
            }
            else
            {
                insertInNode(current, median, newRight, k);
                goesUp = false;
            }
        }
    }

    return goesUp;
}

void BTree::insertInNode(BNode *current, Product *product, BNode *rightChild, int k)
{
    const int count = current->getCount();

    for (int i = count; i > k; --i)
    {
        current->setKey(i, current->getKey(i - 1));
        current->setChild(i + 1, current->getChild(i));
    }

    current->setKey(k, product);
    current->setChild(k + 1, rightChild);
    current->setCount(count + 1);
}

void BTree::splitNode(BNode *current, Product *&median, BNode *&newRight, int k)
{
    const int oldCount = current->getCount();
    const int totalKeys = oldCount + 1;

    Product **tempKeys = new Product *[m];
    BNode **tempChildren = new BNode *[m + 1];

    for (int i = 0; i < oldCount; i++)
    {
        tempKeys[i] = current->getKey(i);
    }

    for (int i = 0; i <= oldCount; i++)
    {
        tempChildren[i] = current->getChild(i);
    }

    for (int i = oldCount; i > k; --i)
    {
        tempKeys[i] = tempKeys[i - 1];
    }
    tempKeys[k] = median;

    for (int i = oldCount + 1; i > k + 1; --i)
    {
        tempChildren[i] = tempChildren[i - 1];
    }
    tempChildren[k + 1] = newRight;

    int medianPos = totalKeys / 2;
    BNode *newPage = new BNode(m);

    int leftCount = medianPos;
    int rightCount = totalKeys - medianPos - 1;

    for (int i = 0; i < leftCount; i++)
    {
        current->setKey(i, tempKeys[i]);
    }
    for (int i = leftCount; i < m; i++)
    {
        current->setKey(i, nullptr);
    }
    for (int i = 0; i <= leftCount; i++)
    {
        current->setChild(i, tempChildren[i]);
    }
    for (int i = leftCount + 1; i <= m; i++)
    {
        current->setChild(i, nullptr);
    }
    current->setCount(leftCount);

    for (int i = 0; i < rightCount; i++)
    {
        newPage->setKey(i, tempKeys[medianPos + 1 + i]);
    }
    for (int i = rightCount; i < m; i++)
    {
        newPage->setKey(i, nullptr);
    }
    for (int i = 0; i <= rightCount; i++)
    {
        newPage->setChild(i, tempChildren[medianPos + 1 + i]);
    }
    for (int i = rightCount + 1; i <= m; i++)
    {
        newPage->setChild(i, nullptr);
    }
    newPage->setCount(rightCount);

    median = tempKeys[medianPos];
    newRight = newPage;

    delete[] tempKeys;
    delete[] tempChildren;
}

std::string BTree::toDot() const
{
    std::ostringstream dot;
    dot << "digraph BTree {\n";
    dot << "  rankdir=TB;\n";
    dot << "  node [shape=record, fontsize=10];\n";

    if (root == nullptr)
    {
        dot << "  empty [label=\"Arbol B vacio\"];\n";
        dot << "}\n";
        return dot.str();
    }

    std::map<const BNode *, int> ids;
    int nextId = 0;
    appendNodeDot(root, dot, ids, nextId);

    dot << "}\n";
    return dot.str();
}