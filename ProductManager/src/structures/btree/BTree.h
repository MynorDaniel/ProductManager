#ifndef BTREE_H
#define BTREE_H

#include "BNode.h"
#include "DataStructure.h"
#include "exceptions/ProductException.h"
#include <string>

class BTree : public DataStructure
{
private:
    BNode *root;
    int m;

public:
    BTree(int order);
    BTree();
    ~BTree();

    BNode* getRoot();
    void setRoot(BNode* newRoot);
    int getOrder() { return m; }
    void setOrder(int order) { m = order; }

    void insert(const Product &product) override;
    void remove(const Product &product) override;
    Product* search(const std::string &dates) override;
    std::string listAscending() const;
    std::string toDot() const;

private:

    bool searchNode(BNode* current, Product* product, int &k);
    BNode* search(Product* product, int &n);
    BNode* search(BNode* current, Product* product, int &n);
    bool push(BNode* current, Product* product, Product*& median, BNode*& newRight);
    void insertInNode(BNode* current, Product* product, BNode* rightChild, int k);
    void splitNode(BNode* current, Product*& median, BNode*& newRight, int k);
    void inOrder(const BNode* node, std::string& output) const;
};

#endif // BTREE_H