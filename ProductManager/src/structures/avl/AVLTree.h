#ifndef AVLTREE_H
#define AVLTREE_H

#include "AVLNode.h"
#include "DataStructure.h"

class AVLTree : public DataStructure
{
private:
    AVLNode* root;
    int size;

public:
    AVLTree();
    ~AVLTree() override;
    void insert(const Product& product) override;
    void remove(const Product& product) override;
    Product* search(const std::string& name) override;
    Product* search(AVLNode* node, const std::string& name);
    std::string inOrder() const;
    std::string inOrder(const AVLNode* node) const;
    AVLNode* getRoot() const;

};

#endif // AVLTREE_H