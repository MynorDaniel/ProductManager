#include "BNode.h"

BNode::BNode(int order)
{
    m = order;
    count = 0;

    keys = new Product *[order];
    for (int k = 0; k < order; k++)
        keys[k] = nullptr;

    children = new BNode *[order + 1];
    for (int k = 0; k <= order; k++)
        children[k] = nullptr;
}

BNode::~BNode()
{
    delete[] keys;
    delete[] children;
}

bool BNode::isFull()
{
    return count == m - 1;
}

bool BNode::isSemiEmpty()
{
    return count < m / 2;
}

Product *BNode::getKey(int index)
{
    if (index < 0 || index >= count)
        return nullptr;
    return keys[index];
}

Product *BNode::getKey(int index) const
{
    if (index < 0 || index >= count)
        return nullptr;
    return keys[index];
}

void BNode::setKey(int index, Product *product)
{
    if (index < 0 || index >= m)
        return;
    keys[index] = product;
}

BNode *BNode::getChild(int index)
{
    if (index < 0 || index > count)
        return nullptr;
    return children[index];
}

BNode *BNode::getChild(int index) const
{
    if (index < 0 || index > count)
        return nullptr;
    return children[index];
}

void BNode::setChild(int index, BNode *child)
{
    if (index < 0 || index > m)
        return;
    children[index] = child;
}

int BNode::getCount()
{
    return count;
}

int BNode::getCount() const
{
    return count;
}

void BNode::setCount(int newCount)
{
    if (newCount >= 0 && newCount <= m)
        count = newCount;
}
