#ifndef MEASURER_H
#define MEASURER_H

#include <string>
#include <chrono>
#include <sstream>
#include <vector>

class ProductManager;
class UnsortedLinkedList;
class SortedLinkedList;
class AVLTree;
struct Product;

class Measurer
{
private:
    static constexpr int kQueriesPerBatch = 5;
    static constexpr int kRunsPerScenario = 20;

    struct PerformanceStats
    {
        double avgSearchUs = 0.0;
        double avgInsertUs = 0.0;
        double avgDeleteUs = 0.0;
    };

    struct ScenarioResult
    {
        std::string name;
        PerformanceStats unsortedList;
        PerformanceStats sortedList;
        PerformanceStats avl;
    };

    void fillFakeProduct(Product &fake, const std::string &name, const std::string &barcode);

    PerformanceStats runUnsortedPerformanceTest(const std::vector<std::string> &queries, const std::string &prefix, UnsortedLinkedList &unsorted);

    PerformanceStats runSortedPerformanceTest(const std::vector<std::string> &queries, const std::string &prefix, SortedLinkedList &sorted);

    PerformanceStats runAVLPerformanceTest(const std::vector<std::string> &queries, const std::string &prefix, AVLTree &avl);

    void appendRow(std::ostringstream &report, const std::string &label, const PerformanceStats &stats);

public:
    Measurer();
    ~Measurer();

    std::string measure(ProductManager* manager);
};

#endif // MEASURER_H