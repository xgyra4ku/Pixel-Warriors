cmake_minimum_required(VERSION 3.5)

project(Pixel-Warriors)

set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/debug)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Debug)

# Укажите пути к исходным файлам и заголовочным файлам
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# Добавьте пути к дополнительным библиотекам и заголовочным файлам
set(JSON_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/json/include)

# Добавьте исходные файлы вашего проекта
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)

# Добавьте заголовочные файлы вашего проекта
file(GLOB_RECURSE HEADERS ${INCLUDE_DIR}/*.hpp)

# Включите заголовочные файлы в проект для удобства IDE
add_executable(Pixel-Warriors ${SOURCES} ${HEADERS})

set(SFML_DIR ${CMAKE_SOURCE_DIR}/SFML)

include_directories(${SFML_DIR}/include)
link_directories(${SFML_DIR}/lib)

# Включите директорию с заголовочными файлами nlohmann/json
include_directories(${JSON_INCLUDE_DIR})

list(APPEND CMAKE_PREFIX_PATH ${SFML_DIR}/lib/cmake/SFML)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

target_link_libraries(Pixel-Warriors sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
