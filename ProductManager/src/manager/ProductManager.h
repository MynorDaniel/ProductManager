#ifndef PRODUCTMANAGER_H
#define PRODUCTMANAGER_H

#include "csv/CSVManager.h"
#include "structures/DataStructure.h"
#include "measurer/Measurer.h"
#include <fstream>

class ProductManager
{
private:
    CSVManager csvManager;
    Measurer measurer; 

public:
    ProductManager();
    ~ProductManager();

    void start();

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