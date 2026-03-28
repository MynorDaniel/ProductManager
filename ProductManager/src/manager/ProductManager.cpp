#include "ProductManager.h"

namespace Test {
    void printStructuresState(const ProductManager& manager);
}

ProductManager::ProductManager()
{
    this->csvManager = CSVManager();
    this->measurer = Measurer();
    this->unsortedList = UnsortedLinkedList();
}

ProductManager::~ProductManager()
{

}

void ProductManager::start()
{
    std::cout << "Bienvenido a Product Manager" << std::endl;

    while (true) {
        int option = showMenu();

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
        std::cout << "Producto agregado exitosamente." << std::endl;
    } catch (const ProductException& e) {
        std::cout << "Error al agregar producto: " << e.what() << std::endl;
    }

    Test::printStructuresState(*this);
}

void ProductManager::removeProduct()
{
    std::cout << "Ingrese el código de barras del producto a eliminar: ";
    std::string barcode;
    std::getline(std::cin, barcode);

    Product product;
    product.barcode = barcode;
    unsortedList.remove(product);
    sortedList.remove(product);
    std::cout << "Producto eliminado." << std::endl;

    Test::printStructuresState(*this);
}

void ProductManager::searchProductByName()
{
    std::cout << "Buscar producto por nombre" << std::endl;
}

void ProductManager::searchProductByCategory()
{
    std::cout << "Buscar producto por categoría" << std::endl;
}

void ProductManager::searchProductByExpirationRange()
{
    std::cout << "Buscar producto por rango de caducidad" << std::endl;
}

void ProductManager::listByName()
{
    std::cout << "Listar por nombre" << std::endl;
}

void ProductManager::compareSearches()
{
    std::cout << "Comparar búsquedas" << std::endl;
}

void ProductManager::loadCSV()
{
    std::cout << "Ingrese la ruta del archivo CSV: ";
    std::string filePath;
    std::getline(std::cin, filePath);
    std::vector<Product> products = csvManager.readCSV(filePath);

    for(int i = 0; i < products.size(); i++) {
        std::cout << "Producto " << (i + 1) << ": " << products[i].name << ", " << products[i].barcode << ", " 
                  << products[i].category << ", " << products[i].expiry_date << ", " << products[i].brand 
                  << ", " << products[i].price << ", " << products[i].stock << std::endl;

        unsortedList.insert(products[i]);
        sortedList.insert(products[i]);
    }

    Logger& logger = csvManager.getLogger();
    std::string logs = logger.getLogs();

    std::cout << "\nLogs de carga del CSV:" << std::endl;
    std::cout << logs << std::endl;


    std::string logsPath = logger.saveLogs("errors.log");
    std::cout << "Logs guardados en: " << logsPath << std::endl;
    logger.clearLogs();

    Test::printStructuresState(*this);
}

void ProductManager::visualizeTrees()
{
    std::cout << "Visualizar árboles" << std::endl;
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
    }
}