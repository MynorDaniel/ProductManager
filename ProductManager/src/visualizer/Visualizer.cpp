#include "Visualizer.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

Visualizer::Visualizer()
{
}

Visualizer::~Visualizer()
{
}

void Visualizer::visualize(ProductManager* manager)
{
    if (manager == nullptr)
    {
        return;
    }

    const std::string imageDirectory = "img";
    std::filesystem::create_directories(imageDirectory);

    std::vector<DotItem> dots;
    dots.push_back({"lista_desordenada", manager->getUnsortedList().toDot()});
    dots.push_back({"lista_ordenada", manager->getSortedList().toDot()});
    dots.push_back({"avl", manager->getAVLTree().toDot()});
    dots.push_back({"arbol_b", manager->getBTree().toDot()});
    dots.push_back({"arbol_bplus", manager->getBPlusTree().toDot()});

    int imageIndex = nextImageIndex(imageDirectory);
    for (std::size_t i = 0; i < dots.size(); i++)
    {
        std::ostringstream baseName;
        baseName << imageDirectory << "/" << dots[i].name << "_" << imageIndex;

        const std::string dotPath = baseName.str() + ".dot";
        const std::string pngPath = baseName.str() + ".png";
    const std::string svgPath = baseName.str() + ".svg";

        if (!writeTextFile(dotPath, dots[i].dot))
        {
            std::cout << "No se pudo escribir: " << dotPath << std::endl;
            imageIndex++;
            continue;
        }

        if (shouldRenderAsSvg(dots[i].dot))
        {
            if (renderSvgFromDot(dotPath, svgPath))
            {
                std::cout << "Imagen generada (SVG por tamaño): " << svgPath << std::endl;
            }
            else
            {
                std::cout << "No se pudo generar SVG para: " << dotPath << " (asegura que Graphviz esté instalado)" << std::endl;
            }
        }
        else
        {
            if (renderPngFromDot(dotPath, pngPath))
            {
                std::cout << "Imagen generada: " << pngPath << std::endl;
            }
            else
            {
                std::cout << "No se pudo generar PNG para: " << dotPath << " (asegura que Graphviz esté instalado)" << std::endl;
            }
        }

        imageIndex++;
    }

    std::cout << "Visualización completada en carpeta /img" << std::endl;
}

int Visualizer::nextImageIndex(const std::string& directory) const
{
    int count = 0;

    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.is_regular_file() && (entry.path().extension() == ".png" || entry.path().extension() == ".svg"))
        {
            count++;
        }
    }

    return count + 1;
}

bool Visualizer::writeTextFile(const std::string& filePath, const std::string& content) const
{
    std::ofstream file(filePath);
    if (!file.is_open())
    {
        return false;
    }

    file << content;
    return true;
}

bool Visualizer::shouldRenderAsSvg(const std::string& dotContent) const
{
    static const std::size_t LARGE_DOT_THRESHOLD = 120000;
    return dotContent.size() >= LARGE_DOT_THRESHOLD;
}

bool Visualizer::renderPngFromDot(const std::string& dotPath, const std::string& pngPath) const
{
    std::string command = "dot -Tpng \"" + dotPath + "\" -o \"" + pngPath + "\"";
    int result = std::system(command.c_str());
    return result == 0;

}

bool Visualizer::renderSvgFromDot(const std::string& dotPath, const std::string& svgPath) const
{
    std::string command = "dot -Tsvg \"" + dotPath + "\" -o \"" + svgPath + "\"";
    int result = std::system(command.c_str());
    return result == 0;
}