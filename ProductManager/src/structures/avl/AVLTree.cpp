#include "AVLTree.h"

namespace {
void clearSubtree(AVLNode* node)
{
    if (node == nullptr) {
        return;
    }

    clearSubtree(node->getLeft());
    clearSubtree(node->getRight());
    delete node;
}
}

AVLTree::AVLTree()
{
    root = nullptr;
    size = 0;
}

AVLTree::~AVLTree()
{
    clearSubtree(root);
    root = nullptr;
    size = 0;
}

AVLNode* AVLTree::getRoot() const
{
    return root;
}

void AVLTree::insert(const Product& product)
{
    // 
}

void AVLTree::remove(const Product& product)
{
    // 
}

Product* AVLTree::search(const std::string& name)
{
    return search(root, name);
}

Product* AVLTree::search(AVLNode* node, const std::string& name)
{
    if (node == nullptr) {
        return nullptr;
    }

    Product* currentProduct = node->getData();
    if (currentProduct != nullptr && currentProduct->name == name) {
        return currentProduct;
    }

    if (name < currentProduct->name) {
        return search(node->getLeft(), name);
    } else {
        return search(node->getRight(), name);
    }
    
}

std::string AVLTree::inOrder() const
{
    return inOrder(root);
}

std::string AVLTree::inOrder(const AVLNode* node) const
{
    if (node == nullptr) {
        return "";
    }

    std::string result = "Lista de productos en AVL:\n";
    result += inOrder(node->getLeft());
    
    Product* product = node->getData();
    if (product != nullptr) {
        result += product->name + ", " + product->barcode + ", " 
                + product->category + ", " + product->expiry_date + ", " 
                + product->brand + ", " + std::to_string(product->price) 
                + ", " + std::to_string(product->stock) + "\n";
    }

    result += inOrder(node->getRight());
    return result;
}