#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "manager/ProductManager.h"
#include <string>
#include <vector>

class Visualizer
{
public:
    Visualizer();
    ~Visualizer();

    void visualize(ProductManager* manager);


private:
    struct DotItem
    {
        std::string name;
        std::string dot;
    };

    int nextImageIndex(const std::string& directory) const;
    bool writeTextFile(const std::string& filePath, const std::string& content) const;
    bool shouldRenderAsSvg(const std::string& dotContent) const;
    bool renderPngFromDot(const std::string& dotPath, const std::string& pngPath) const;
    bool renderSvgFromDot(const std::string& dotPath, const std::string& svgPath) const;
};

#endif // VISUALIZER_H