#include "AVLNode.h"

AVLNode::AVLNode(Product* product)
{
    data = product;
    left = right = NULL;
    fe = 0;
}

AVLNode::AVLNode(Product* product, int vfe)
{
    data = product;
    left = right = NULL;
    fe = vfe;
}

AVLNode::AVLNode(AVLNode* leftN, Product* product, AVLNode* rightN)
{
    data = product;
    left = leftN;
    right = rightN;
    fe = 0;
}

AVLNode::AVLNode(AVLNode* leftN, int vfe, Product* valor, AVLNode* rightN)
{
    data = valor;
    left = leftN;
    right = rightN;
    fe = vfe;
}

AVLNode::~AVLNode()
{
    delete data;
}

Product* AVLNode::getData(){ return data; }

Product* AVLNode::getData() const { return data; }

AVLNode* AVLNode::getLeft(){ return left; }

AVLNode* AVLNode::getLeft() const { return left; }

AVLNode* AVLNode::getRight(){ return right; }

AVLNode* AVLNode::getRight() const { return right; }

void AVLNode::setData(Product* d){ data = d; }

void AVLNode::setLeft(AVLNode* n){ left = n; }

void AVLNode::setRight(AVLNode* n){ right = n; }

void AVLNode::setFe(int vfe) { fe = vfe;}

int AVLNode::getFe(){ return fe;}