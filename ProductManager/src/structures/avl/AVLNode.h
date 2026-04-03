#ifndef AVLNODE_H
#define AVLNODE_H

#include "product/Product.h"

class AVLNode
{
private:

    Product* data;
    AVLNode *left;
    AVLNode *right;
    int fe;

public:

    AVLNode(Product* product);
    AVLNode(Product* product, int vfe);
    AVLNode(const Product* product);
    AVLNode(const Product* product, int vfe);
    AVLNode(AVLNode* leftN, Product* product, AVLNode* rightN);
    AVLNode(AVLNode* leftN, int vfe, Product* valor, AVLNode* rightN);
    ~AVLNode();

    Product* getData();
    Product* getData() const;
    AVLNode* getLeft();
    AVLNode* getLeft() const;
    AVLNode* getRight();
    AVLNode* getRight() const;
    void setData(Product* d);
    void setLeft(AVLNode* n);
    void setRight(AVLNode* n);
    void setFe(int vfe);
    int getFe();
};

#endif // AVLNODE_H