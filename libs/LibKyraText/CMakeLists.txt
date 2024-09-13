cmake_minimum_required(VERSION 3.10)
project(LibKyraText)


set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML")

include_directories("${SFML_DIR}/include")
link_directories("${SFML_DIR}/lib")

add_library(LibKyraText STATIC src/LibKyraText.cpp
        include/LibKyraText.h)
target_include_directories(LibKyraText PUBLIC include)
list(APPEND CMAKE_PREFIX_PATH "${SFML_DIR}/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(
        LibKyraText
        PRIVATE
        sfml-system
        sfml-window
        sfml-graphics
        sfml-network
        sfml-audio
)
