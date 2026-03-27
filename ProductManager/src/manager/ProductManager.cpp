#include "ProductManager.h"
#include <iostream>
#include <limits>

ProductManager::ProductManager()
{
    this->csvManager = CSVManager();
    this->measurer = Measurer();
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
    std::cout << "[1] Agregar producto" << std::endl;
}

void ProductManager::removeProduct()
{
    std::cout << "[2] Eliminar producto" << std::endl;
}

void ProductManager::searchProductByName()
{
    std::cout << "[3] Buscar producto por nombre" << std::endl;
}

void ProductManager::searchProductByCategory()
{
    std::cout << "[4] Buscar producto por categoría" << std::endl;
}

void ProductManager::searchProductByExpirationRange()
{
    std::cout << "[5] Buscar producto por rango de caducidad" << std::endl;
}

void ProductManager::listByName()
{
    std::cout << "[6] Listar por nombre" << std::endl;
}

void ProductManager::compareSearches()
{
    std::cout << "[7] Comparar búsquedas" << std::endl;
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
    }

    Logger& logger = csvManager.getLogger();
    std::string logs = logger.getLogs();

    std::cout << "\nLogs de carga del CSV:" << std::endl;
    std::cout << logs << std::endl;


    std::string logsPath = logger.saveLogs("errors.log");
    std::cout << "Logs guardados en: " << logsPath << std::endl;
    logger.clearLogs();
}

void ProductManager::visualizeTrees()
{
    std::cout << "[9] Visualizar árboles" << std::endl;
}

void ProductManager::exitProgram()
{
    std::cout << "Saliendo..." << std::endl;
}