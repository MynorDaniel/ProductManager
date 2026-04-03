#include "Measurer.h"

#include "manager/ProductManager.h"

#include <iomanip>
#include <sstream>
#include <vector>

void Measurer::fillFakeProduct(Product &fake, const std::string &name, const std::string &barcode)
{
    fake.name = name;
    fake.barcode = barcode;
    fake.category = "-";
    fake.expiry_date = "2099-12-31";
    fake.brand = "-";
    fake.price = 1.0;
    fake.stock = 1;
}

Measurer::PerformanceStats Measurer::runUnsortedPerformanceTest(const std::vector<std::string> &queries, const std::string &prefix, UnsortedLinkedList &unsorted)
{
    PerformanceStats stats;
    if (queries.empty())
    {
        return stats;
    }

    double totalSearchUs = 0.0;
    double totalInsertUs = 0.0;
    double totalDeleteUs = 0.0;

    for (int run = 0; run < kRunsPerScenario; run++)
    {
        for (int q = 0; q < static_cast<int>(queries.size()); q++)
        {
            const std::string &query = queries[q];

            // Medición de latencia de búsqueda para la consulta actual
            auto searchStart = std::chrono::high_resolution_clock::now();
            unsorted.search(query);
            auto searchEnd = std::chrono::high_resolution_clock::now();
            totalSearchUs += std::chrono::duration<double, std::micro>(searchEnd - searchStart).count();

            Product fake;
            fillFakeProduct(fake, prefix + "_NAME_" + std::to_string(run) + "_" + std::to_string(q),
                            prefix + "_BAR_" + std::to_string(run) + "_" + std::to_string(q));

            // Inserción de un registro
            auto insertStart = std::chrono::high_resolution_clock::now();
            unsorted.insert(fake);
            auto insertEnd = std::chrono::high_resolution_clock::now();
            totalInsertUs += std::chrono::duration<double, std::micro>(insertEnd - insertStart).count();

            // Eliminación del registro
            auto deleteStart = std::chrono::high_resolution_clock::now();
            unsorted.remove(fake);
            auto deleteEnd = std::chrono::high_resolution_clock::now();
            totalDeleteUs += std::chrono::duration<double, std::micro>(deleteEnd - deleteStart).count();
        }
    }

    const double totalOps = static_cast<double>(kRunsPerScenario * static_cast<int>(queries.size()));
    stats.avgSearchUs = totalSearchUs / totalOps;
    stats.avgInsertUs = totalInsertUs / totalOps;
    stats.avgDeleteUs = totalDeleteUs / totalOps;
    return stats;
}

Measurer::PerformanceStats Measurer::runSortedPerformanceTest(const std::vector<std::string> &queries, const std::string &prefix, SortedLinkedList &sorted)
{
    PerformanceStats stats;
    if (queries.empty())
    {
        return stats;
    }

    double totalSearchUs = 0.0;
    double totalInsertUs = 0.0;
    double totalDeleteUs = 0.0;

    for (int run = 0; run < kRunsPerScenario; run++)
    {
        for (int q = 0; q < static_cast<int>(queries.size()); q++)
        {
            const std::string &query = queries[q];

            auto searchStart = std::chrono::high_resolution_clock::now();
            sorted.search(query);
            auto searchEnd = std::chrono::high_resolution_clock::now();
            totalSearchUs += std::chrono::duration<double, std::micro>(searchEnd - searchStart).count();

            Product fake;
            fillFakeProduct(fake,
                            prefix + "_NAME_" + std::to_string(run) + "_" + std::to_string(q),
                            prefix + "_BAR_" + std::to_string(run) + "_" + std::to_string(q));

            auto insertStart = std::chrono::high_resolution_clock::now();
            sorted.insert(fake);
            auto insertEnd = std::chrono::high_resolution_clock::now();
            totalInsertUs += std::chrono::duration<double, std::micro>(insertEnd - insertStart).count();

            auto deleteStart = std::chrono::high_resolution_clock::now();
            sorted.remove(fake);
            auto deleteEnd = std::chrono::high_resolution_clock::now();
            totalDeleteUs += std::chrono::duration<double, std::micro>(deleteEnd - deleteStart).count();
        }
    }

    const double totalOps = static_cast<double>(kRunsPerScenario * static_cast<int>(queries.size()));
    stats.avgSearchUs = totalSearchUs / totalOps;
    stats.avgInsertUs = totalInsertUs / totalOps;
    stats.avgDeleteUs = totalDeleteUs / totalOps;
    return stats;
}

Measurer::PerformanceStats Measurer::runAVLPerformanceTest(const std::vector<std::string> &queries,const std::string &prefix, AVLTree &avl)
{
    PerformanceStats stats;
    if (queries.empty())
    {
        return stats;
    }

    double totalSearchUs = 0.0;
    double totalInsertUs = 0.0;
    double totalDeleteUs = 0.0;

    for (int run = 0; run < kRunsPerScenario; run++)
    {
        for (int q = 0; q < static_cast<int>(queries.size()); q++)
        {
            const std::string &query = queries[q];

            auto searchStart = std::chrono::high_resolution_clock::now();
            avl.search(query);
            auto searchEnd = std::chrono::high_resolution_clock::now();
            totalSearchUs += std::chrono::duration<double, std::micro>(searchEnd - searchStart).count();

            Product fake;
            fillFakeProduct(fake,
                            prefix + "_NAME_" + std::to_string(run) + "_" + std::to_string(q),
                            prefix + "_BAR_" + std::to_string(run) + "_" + std::to_string(q));

            auto insertStart = std::chrono::high_resolution_clock::now();
            avl.insert(fake);
            auto insertEnd = std::chrono::high_resolution_clock::now();
            totalInsertUs += std::chrono::duration<double, std::micro>(insertEnd - insertStart).count();

            // Eliminación del registro temporal para medir costo de baja.
            auto deleteStart = std::chrono::high_resolution_clock::now();
            avl.remove(fake);
            auto deleteEnd = std::chrono::high_resolution_clock::now();
            totalDeleteUs += std::chrono::duration<double, std::micro>(deleteEnd - deleteStart).count();
        }
    }

    const double totalOps = static_cast<double>(kRunsPerScenario * static_cast<int>(queries.size()));
    stats.avgSearchUs = totalSearchUs / totalOps;
    stats.avgInsertUs = totalInsertUs / totalOps;
    stats.avgDeleteUs = totalDeleteUs / totalOps;
    return stats;
}

void Measurer::appendRow(std::ostringstream &report, const std::string &label, const PerformanceStats &stats)
{
    report << "| " << std::left << std::setw(30) << label
           << "| " << std::right << std::setw(12) << std::fixed << std::setprecision(2) << stats.avgSearchUs
           << "| " << std::setw(12) << stats.avgInsertUs
           << "| " << std::setw(12) << stats.avgDeleteUs << "|\n";
}

Measurer::Measurer()
{
}

Measurer::~Measurer()
{
}

std::string Measurer::measure(ProductManager* manager)
{
    if (manager == nullptr)
    {
        return "Error: manager nulo.";
    }

    AVLTree &avl = const_cast<AVLTree &>(manager->getAVLTree());
    UnsortedLinkedList &unsorted = const_cast<UnsortedLinkedList &>(manager->getUnsortedList());
    SortedLinkedList &sorted = const_cast<SortedLinkedList &>(manager->getSortedList());

    std::vector<std::string> barcodes;
    std::vector<std::string> names;

    for (LinkedListNode *current = unsorted.getHead(); current != nullptr; current = current->getNext())
    {
        Product *product = current->getData();
        if (product != nullptr)
        {
            barcodes.push_back(product->barcode);
            names.push_back(product->name);
        }
    }

    if (barcodes.empty() || names.empty())
    {
        return "No hay datos cargados para medir rendimiento. Carga productos antes de ejecutar la medición.";
    }

    std::string sortedFirst = barcodes.front();
    std::string sortedLast = barcodes.front();
    LinkedListNode *sortedHead = sorted.getHead();
    if (sortedHead != nullptr)
    {
        Product *headProduct = sortedHead->getData();
        if (headProduct != nullptr)
        {
            sortedFirst = headProduct->barcode;
        }

        LinkedListNode *tail = sortedHead;
        while (tail->getNext() != nullptr)
        {
            tail = tail->getNext();
        }

        Product *tailProduct = tail->getData();
        if (tailProduct != nullptr)
        {
            sortedLast = tailProduct->barcode;
        }
    }

    std::string avlFirst = names.front();
    std::string avlLast = names.front();
    AVLNode *avlRoot = avl.getRoot();
    if (avlRoot != nullptr)
    {
        AVLNode *leftMost = avlRoot;
        while (leftMost->getLeft() != nullptr)
        {
            leftMost = leftMost->getLeft();
        }

        Product *leftProduct = leftMost->getData();
        if (leftProduct != nullptr)
        {
            avlFirst = leftProduct->name;
        }

        AVLNode *rightMost = avlRoot;
        while (rightMost->getRight() != nullptr)
        {
            rightMost = rightMost->getRight();
        }

        Product *rightProduct = rightMost->getData();
        if (rightProduct != nullptr)
        {
            avlLast = rightProduct->name;
        }
    }

    // Escenarios
    std::vector<std::pair<std::string, std::vector<std::string>>> listScenarios;
    std::vector<std::string> listSuccess;
    for (int i = 0; i < kQueriesPerBatch; i++)
    {
        listSuccess.push_back(barcodes[i % barcodes.size()]);
    }

    std::vector<std::string> listFailed;
    for (int i = 0; i < kQueriesPerBatch; i++)
    {
        listFailed.push_back("NO_BARCODE_" + std::to_string(i));
    }

    std::vector<std::string> listExtremes;
    for (int i = 0; i < kQueriesPerBatch; i++)
    {
        listExtremes.push_back((i % 2 == 0) ? sortedFirst : sortedLast);
    }

    listScenarios.push_back({"Busqueda exitosa (fija)", listSuccess});
    listScenarios.push_back({"Busqueda fallida", listFailed});
    listScenarios.push_back({"Busqueda extremos (primero/ultimo)", listExtremes});

    std::vector<std::pair<std::string, std::vector<std::string>>> avlScenarios;
    std::vector<std::string> avlSuccess;
    for (int i = 0; i < kQueriesPerBatch; i++)
    {
        avlSuccess.push_back(names[i % names.size()]);
    }

    std::vector<std::string> avlFailed;
    for (int i = 0; i < kQueriesPerBatch; i++)
    {
        avlFailed.push_back("NO_NAME_" + std::to_string(i));
    }

    std::vector<std::string> avlExtremes;
    for (int i = 0; i < kQueriesPerBatch; i++)
    {
        avlExtremes.push_back((i % 2 == 0) ? avlFirst : avlLast);
    }

    avlScenarios.push_back({"Busqueda exitosa (fija)", avlSuccess});
    avlScenarios.push_back({"Busqueda fallida", avlFailed});
    avlScenarios.push_back({"Busqueda extremos (primero/ultimo)", avlExtremes});

    std::vector<ScenarioResult> results;

    for (int i = 0; i < static_cast<int>(listScenarios.size()); i++)
    {
        ScenarioResult scenario;
        scenario.name = listScenarios[i].first;

        const std::vector<std::string> &listQueries = listScenarios[i].second;
        const std::vector<std::string> &avlQueries = avlScenarios[i].second;

        scenario.unsortedList = runUnsortedPerformanceTest(listQueries, "UNSORTED_" + std::to_string(i), unsorted);
        scenario.sortedList = runSortedPerformanceTest(listQueries, "SORTED_" + std::to_string(i), sorted);
        scenario.avl = runAVLPerformanceTest(avlQueries, "AVL_" + std::to_string(i), avl);

        results.push_back(scenario);
    }

    PerformanceStats overallUnsorted;
    PerformanceStats overallSorted;
    PerformanceStats overallAVL;

    for (const ScenarioResult &result : results)
    {
        overallUnsorted.avgSearchUs += result.unsortedList.avgSearchUs;
        overallUnsorted.avgInsertUs += result.unsortedList.avgInsertUs;
        overallUnsorted.avgDeleteUs += result.unsortedList.avgDeleteUs;

        overallSorted.avgSearchUs += result.sortedList.avgSearchUs;
        overallSorted.avgInsertUs += result.sortedList.avgInsertUs;
        overallSorted.avgDeleteUs += result.sortedList.avgDeleteUs;

        overallAVL.avgSearchUs += result.avl.avgSearchUs;
        overallAVL.avgInsertUs += result.avl.avgInsertUs;
        overallAVL.avgDeleteUs += result.avl.avgDeleteUs;
    }

    const double scenarioCount = static_cast<double>(results.size());
    overallUnsorted.avgSearchUs /= scenarioCount;
    overallUnsorted.avgInsertUs /= scenarioCount;
    overallUnsorted.avgDeleteUs /= scenarioCount;

    overallSorted.avgSearchUs /= scenarioCount;
    overallSorted.avgInsertUs /= scenarioCount;
    overallSorted.avgDeleteUs /= scenarioCount;

    overallAVL.avgSearchUs /= scenarioCount;
    overallAVL.avgInsertUs /= scenarioCount;
    overallAVL.avgDeleteUs /= scenarioCount;

    std::ostringstream report;
    report << "REPORTE DE MEDICION\n";
    report << "Muestras: " << barcodes.size() << " productos\n";
    report << "Configuracion: " << kQueriesPerBatch << " consultas por escenario, "
           << kRunsPerScenario << " repeticiones por escenario.\n";
    report << "Unidad principal: microsegundos (us).\n\n";

    for (const ScenarioResult &result : results)
    {
        report << "Escenario: " << result.name << "\n";
        report << "| Estructura                     | Prom. busqueda (us) | Prom. insercion (us) | Prom. eliminacion (us) |\n";
        report << "|-------------------------------|----------------:|----------------:|----------------:|\n";
     appendRow(report, "Lista enlazada desordenada", result.unsortedList);
     appendRow(report, "Lista enlazada ordenada", result.sortedList);
     appendRow(report, "AVL (busqueda por nombre)", result.avl);
        report << "\n";
    }

    report << "Resumen\n";
    report << "| Estructura                     | Prom. busqueda (us) | Prom. insercion (us) | Prom. eliminacion (us) |\n";
    report << "|-------------------------------|----------------:|----------------:|----------------:|\n";
    appendRow(report, "Lista enlazada desordenada", overallUnsorted);
    appendRow(report, "Lista enlazada ordenada", overallSorted);
    appendRow(report, "AVL (busqueda por nombre)", overallAVL);
    report << "\n";

    report << "Complejidades:\n";
    report << "- Lista enlazada desordenada (busqueda secuencial por barcode): O(n)\n";
    report << "- Lista enlazada ordenada (busqueda secuencial por barcode): O(n)\n";
    report << "- AVL (busqueda por nombre): O(log n)\n";
    report << "- Insercion/Eliminacion en AVL: O(log n)\n";
    report << "- Insercion en lista desordenada: O(1), eliminacion: O(n)\n";
    report << "- Insercion en lista ordenada: O(n), eliminacion: O(n)\n\n";

    return report.str();
}
