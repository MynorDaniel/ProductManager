#include "ProductManager.h"
#include "visualizer/Visualizer.h"

namespace Test {
    void printStructuresState(const ProductManager& manager);
    void printTree(AVLNode* node, int space = 0, int indent = 5);
}

ProductManager::ProductManager()
{
    this->csvManager = CSVManager();
    this->measurer = Measurer();
    this->unsortedList = UnsortedLinkedList();
    this->avlTree = AVLTree();
    this->bTree = BTree();
    this->bPlusTree = BPlusTree(5);
}

ProductManager::~ProductManager()
{

}

void ProductManager::start()
{
    std::cout << "Bienvenido a Product Manager" << std::endl;

    while (true) {
        int option = showMenu();

        try {
            switch (option) {
                case 1:
                    addProduct();
                    break;
                case 2:
                    removeProduct();
                    break;
                case 3:
                    searchProductByName();
                    break;
                case 4:
                    searchProductByCategory();
                    break;
                case 5:
                    searchProductByExpirationRange();
                    break;
                case 6:
                    listByName();
                    break;
                case 7:
                    compareSearches();
                    break;
                case 8:
                    loadCSV();
                    break;
                case 9:
                    visualizeTrees();
                    break;
                case 0:
                    exitProgram();
                    return;
                default:
                    std::cout << "Opción inválida." << std::endl;
                    break;
            }
        } catch (const ProductException& e) {
            std::cout << "Error de producto: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Error inesperado: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "Error inesperado no identificado." << std::endl;
        }
    }
}

int ProductManager::showMenu()
{
    std::cout << "\n--- Menú ---\n"
              << "1. Agregar producto\n"
              << "2. Eliminar producto\n"
              << "3. Buscar producto por nombre\n"
              << "4. Buscar producto por categoría\n"
              << "5. Buscar producto por rango de caducidad\n"
              << "6. Listar por nombre\n"
              << "7. Comparar búsquedas\n"
              << "8. Cargar CSV\n"
              << "9. Visualizar árboles\n"
              << "0. Salir\n"
              << "Seleccione una opción (0-9): ";

    int option;
    while (!(std::cin >> option) || option < 0 || option > 9) {
        std::cout << "Opción inválida. Intente de nuevo (0-9): ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return option;
}

void ProductManager::addProduct()
{
    std::cout << "Ingrese el nombre del producto: ";
    std::string name;
    std::getline(std::cin, name);

    std::cout << "Ingrese el código de barras del producto: ";
    std::string barcode;
    std::getline(std::cin, barcode);

    std::cout << "Ingrese la categoría del producto: ";
    std::string category;
    std::getline(std::cin, category);

    std::cout << "Ingrese la fecha de caducidad del producto (YYYY-MM-DD): ";
    std::string expiry_date;
    std::getline(std::cin, expiry_date);

    std::cout << "Ingrese la marca del producto: ";
    std::string brand;
    std::getline(std::cin, brand);

    std::cout << "Ingrese el precio del producto: ";
    double price;
    std::cin >> price;
    std::cin.ignore();

    std::cout << "Ingrese el stock del producto: ";
    int stock;
    std::cin >> stock;
    std::cin.ignore();

    Product product;
    product.name = name;
    product.barcode = barcode;
    product.category = category;
    product.expiry_date = expiry_date;
    product.brand = brand;
    product.price = price;
    product.stock = stock;

    try {
        product.validate();
        unsortedList.insert(product);
        sortedList.insert(product);
        avlTree.insert(product);
        bTree.insert(product);
        bPlusTree.insert(product);
        std::cout << "Producto agregado exitosamente." << std::endl;
    } catch (const ProductException& e) {
        std::cout << "Error al agregar producto: " << e.what() << std::endl;
    }

    //Test::printStructuresState(*this);
}

void ProductManager::removeProduct()
{
    std::cout << "Ingrese el código de barras del producto a eliminar: ";
    std::string barcode;
    std::getline(std::cin, barcode);

    Product* foundProduct = unsortedList.search(barcode);
    if (foundProduct == nullptr)
    {
        std::cout << "No se encontró un producto con ese código de barras." << std::endl;
        return;
    }

    Product productToRemove = *foundProduct;

    unsortedList.remove(productToRemove);
    sortedList.remove(productToRemove);
    avlTree.remove(productToRemove);
    bTree.remove(productToRemove);
    bPlusTree.remove(productToRemove);
    std::cout << "Producto eliminado." << std::endl;

    //Test::printStructuresState(*this);
}

void ProductManager::searchProductByName()
{
    std::cout << "Ingrese el nombre del producto a buscar: ";
    std::string name;
    std::getline(std::cin, name);

    Product *foundProduct = avlTree.search(name);
    if (foundProduct == nullptr)
    {
        std::cout << "No se encontró un producto con ese nombre." << std::endl;
        return;
    }

    std::cout << "Producto encontrado: "
              << foundProduct->name << ", "
              << foundProduct->barcode << ", "
              << foundProduct->category << ", "
              << foundProduct->expiry_date << ", "
              << foundProduct->brand << ", "
              << foundProduct->price << ", "
              << foundProduct->stock << std::endl;
}

void ProductManager::searchProductByCategory()
{
    std::cout << "Ingrese la categoría del producto a buscar: ";
    std::string category;
    std::getline(std::cin, category);

    Product *foundProduct = bPlusTree.search(category);
    if (foundProduct == nullptr)
    {
        std::cout << "No se encontró un producto con esa categoría." << std::endl;
        return;
    }

    std::cout << "Producto encontrado: "
              << foundProduct->name << ", "
              << foundProduct->barcode << ", "
              << foundProduct->category << ", "
              << foundProduct->expiry_date << ", "
              << foundProduct->brand << ", "
              << foundProduct->price << ", "
              << foundProduct->stock << std::endl;
}

void ProductManager::searchProductByExpirationRange()
{
    std::cout << "Ingrese el rango de fechas (YYYY-MM-DD#YYYY-MM-DD): ";
    std::string dates;
    std::getline(std::cin, dates);

    Product *foundProduct = bTree.search(dates);
    if (foundProduct == nullptr)
    {
        std::cout << "No se encontraron productos en ese rango o el formato es inválido." << std::endl;
        return;
    }

    std::cout << "Primer producto encontrado en el rango: "
              << foundProduct->name << ", "
              << foundProduct->barcode << ", "
              << foundProduct->category << ", "
              << foundProduct->expiry_date << ", "
              << foundProduct->brand << ", "
              << foundProduct->price << ", "
              << foundProduct->stock << std::endl;
}

void ProductManager::listByName()
{
    std::cout << avlTree.inOrder() << std::endl;
}

void ProductManager::compareSearches()
{
    std::string report = measurer.measure(this);
    std::cout << report << std::endl;
}

void ProductManager::loadCSV()
{
    std::cout << "Ingrese la ruta del archivo CSV: ";
    std::string filePath;
    std::getline(std::cin, filePath);
    std::vector<Product> products = csvManager.readCSV(filePath);

    Logger& logger = csvManager.getLogger();

    for(int i = 0; i < products.size(); i++) {
        std::cout << "Producto " << (i + 1) << ": " << products[i].name << ", " << products[i].barcode << ", " 
                  << products[i].category << ", " << products[i].expiry_date << ", " << products[i].brand 
                  << ", " << products[i].price << ", " << products[i].stock << std::endl;

        try
        {
            unsortedList.insert(products[i]);
            sortedList.insert(products[i]);
            avlTree.insert(products[i]);
            bTree.insert(products[i]);
            bPlusTree.insert(products[i]);
        }
        catch (const ProductException& e)
        {
            logger.addLog(std::string("No se pudo insertar producto '") + products[i].name + "': " + e.what());
            continue;
        }
        catch (const std::exception& e)
        {
            logger.addLog(std::string("Error inesperado al insertar producto '") + products[i].name + "': " + e.what());
            continue;
        }
        catch (...)
        {
            logger.addLog(std::string("Error no identificado al insertar producto '") + products[i].name + "'.");
            continue;
        }
    }

    std::string logs = logger.getLogs();

    std::cout << "\nLogs de carga del CSV:" << std::endl;
    std::cout << logs << std::endl;


    std::string logsPath = logger.saveLogs("errors.log");
    std::cout << "Logs guardados en: " << logsPath << std::endl;
    logger.clearLogs();

    //Test::printStructuresState(*this);
}

void ProductManager::visualizeTrees()
{
    Visualizer visualizer;
    visualizer.visualize(this);
}

void ProductManager::exitProgram()
{
    std::cout << "Saliendo..." << std::endl;
}

const UnsortedLinkedList& ProductManager::getUnsortedList() const {
    return this->unsortedList;
}

const SortedLinkedList& ProductManager::getSortedList() const {
    return this->sortedList;
}

const AVLTree& ProductManager::getAVLTree() const {
    return this->avlTree;
}

const BTree& ProductManager::getBTree() const {
    return this->bTree;
}

const BPlusTree& ProductManager::getBPlusTree() const {
    return this->bPlusTree;
}

namespace Test {
    void printStructuresState(const ProductManager& manager) {
        std::cout << "\nProductos en la lista enlazada no ordenada:" << std::endl;
        for (LinkedListNode* current = manager.getUnsortedList().getHead(); current != nullptr; current = current->getNext()) {
            Product* product = current->getData();
            if (product != nullptr) {
                std::cout << "Producto: " << product->name << ", " << product->barcode << ", " 
                          << product->category << ", " << product->expiry_date << ", " << product->brand 
                          << ", " << product->price << ", " << product->stock << std::endl;
            }
        }

        std::cout << "\nProductos en la lista enlazada ordenada:" << std::endl;
        for (LinkedListNode* current = manager.getSortedList().getHead(); current != nullptr; current = current->getNext()) {
            Product* product = current->getData();
            if (product != nullptr) {
                std::cout << "Producto: " << product->name << ", " << product->barcode << ", " 
                          << product->category << ", " << product->expiry_date << ", " << product->brand 
                          << ", " << product->price << ", " << product->stock << std::endl;
            }
        }

        std::string avlProducts = manager.getAVLTree().inOrder();
        std::cout << avlProducts << std::endl;
        std::cout << "\nÁrbol AVL (visualización):" << std::endl;
        printTree(manager.getAVLTree().getRoot(), 0, 5);
        std::cout << "\nÁrbol B (visualización):" << std::endl;
        std::cout << manager.getBTree().listAscending() << std::endl;
        std::cout << "\nÁrbol B+ (visualización):" << std::endl;
        std::cout << manager.getBPlusTree().listProducts() << std::endl;
    }

    void printTree(AVLNode* node, int space, int indent) {
        if (!node) return;

        space += indent;

        printTree(node->getRight(), space);

        std::cout << std::endl;
        for (int i = indent; i < space; i++) std::cout << " ";
        Product* product = node->getData();
        std::cout << (product ? product->name : "(null)") << "\n";

        printTree(node->getLeft(), space);
    }
}