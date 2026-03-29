#ifndef PRODUCTMANAGER_H
#define PRODUCTMANAGER_H

#include "csv/CSVManager.h"
#include "structures/DataStructure.h"
#include "measurer/Measurer.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <limits>

#include "structures/list/UnsortedLinkedList.h"
#include "structures/list/SortedLinkedList.h"
#include "structures/avl/AVLTree.h"

class ProductManager
{
private:
    CSVManager csvManager;
    Measurer measurer; 

    UnsortedLinkedList unsortedList;
    SortedLinkedList sortedList;
    AVLTree avlTree;

public:
    ProductManager();
    ~ProductManager();

    void start();

    // getters
    const UnsortedLinkedList& getUnsortedList() const;
    const SortedLinkedList& getSortedList() const;
    const AVLTree& getAVLTree() const;

private:

    int showMenu();

    void addProduct();
    void removeProduct();
    void searchProductByName();
    void searchProductByCategory();
    void searchProductByExpirationRange();
    void listByName();
    void compareSearches();
    void loadCSV();
    void visualizeTrees();
    void exitProgram();

};

#endif // PRODUCTMANAGER_H