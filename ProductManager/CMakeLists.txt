cmake_minimum_required(VERSION 3.10.0)
project(ProductManager VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ProductManager
    src/main.cpp
    src/manager/ProductManager.cpp
    src/csv/CSVManager.cpp
    src/csv/Logger.cpp
    src/exceptions/CSVException.cpp
    src/exceptions/ProductException.cpp
    src/product/Product.cpp
    src/measurer/Measurer.cpp
    src/visualizer/Visualizer.cpp
)

target_include_directories(ProductManager PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/csv
    ${CMAKE_CURRENT_SOURCE_DIR}/src/product
    ${CMAKE_CURRENT_SOURCE_DIR}/src/exceptions
    ${CMAKE_CURRENT_SOURCE_DIR}/src/manager
    ${CMAKE_CURRENT_SOURCE_DIR}/src/structures
    ${CMAKE_CURRENT_SOURCE_DIR}/src/measurer
    ${CMAKE_CURRENT_SOURCE_DIR}/src/visualizer
)

