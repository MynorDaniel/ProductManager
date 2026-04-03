#ifndef AVLTREE_H
#define AVLTREE_H

#include "AVLNode.h"
#include "DataStructure.h"
#include "ProductException.h"

class AVLTree : public DataStructure
{
private:
    AVLNode* root;

    AVLNode* rotateRR(AVLNode* n, AVLNode* n1);
    AVLNode* rotateLL(AVLNode* n, AVLNode* n1);
    AVLNode* rotateRL(AVLNode* n, AVLNode* n1);
    AVLNode* rotateLR(AVLNode* n, AVLNode* n1);
    AVLNode* insert(AVLNode* node, const Product* product, bool& grew);
    AVLNode* remove(AVLNode* node, const Product* product, bool& decreased);

public:
    AVLTree();
    ~AVLTree() override;
    void insert(const Product& product) override;
    void remove(const Product& product) override;
    Product* search(const std::string& name) override;
    Product* search(AVLNode* node, const std::string& name);
    std::string inOrder() const;
    std::string inOrder(const AVLNode* node) const;
    std::string toDot() const;
    AVLNode* getRoot() const;

};

#endif // AVLTREE_H