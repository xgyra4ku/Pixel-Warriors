cmake_minimum_required(VERSION 3.5)

project(Pixel-Warriors)

#set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/Debug")
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Укажите пути к исходным файлам и заголовочным файлам
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

# Добавьте пути к дополнительным библиотекам и заголовочным файлам
set(JSON_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/json/include")

# Добавьте исходные файлы вашего проекта
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")

# Добавьте заголовочные файлы вашего проекта
file(GLOB_RECURSE HEADERS "${INCLUDE_DIR}/*.hpp")

# Включите заголовочные файлы в проект для удобства IDE
#add_library(${TARGET_NAME_XXX} SHARED ${SOME_DIR_HAVING_THIS_WARNING}/empty_xxx.c)

add_subdirectory(libs/LibKyraText/)
add_executable(Pixel-Warriors ${SOURCES} ${HEADERS})

set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML")

include_directories("${SFML_DIR}/include")
link_directories("${SFML_DIR}/lib")

# Включите директорию с заголовочными файлами nlohmann/json
include_directories("${JSON_INCLUDE_DIR}")

list(APPEND CMAKE_PREFIX_PATH "${SFML_DIR}/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

target_link_libraries(
        Pixel-Warriors
        sfml-system
        sfml-window
        sfml-graphics
        sfml-network
        sfml-audio
        LibKyraText
)

if(WIN32)
    # Добавляем путь для поиска динамических библиотек во время исполнения
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

    add_custom_command(TARGET Pixel-Warriors POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Dependency"
            $<TARGET_FILE_DIR:Pixel-Warriors>)
endif()

#
## Путь к DLL-файлам в папке сборки
#set(SFML_BIN_DIR ${SFML}"/")
#
## Копируйте DLL в директорию с исполняемым файлом
#add_custom_command(TARGET Pixel-Warriors POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${SFML_BIN_DIR}/sfml-system-2.dll"
#        "${SFML_BIN_DIR}/sfml-window-2.dll"
#        "${SFML_BIN_DIR}/sfml-graphics-2.dll"
#        "${SFML_BIN_DIR}/sfml-network-2.dll"
#        "${SFML_BIN_DIR}/sfml-audio-2.dll"
#        "${SFML_BIN_DIR}/sfml-system-d-2.dll"
#        "${SFML_BIN_DIR}/sfml-window-d-2.dll"
#        "${SFML_BIN_DIR}/sfml-graphics-d-2.dll"
#        "${SFML_BIN_DIR}/sfml-network-d-2.dll"
#        "${SFML_BIN_DIR}/sfml-audio-d-2.dll"
#        $<TARGET_FILE_DIR:Pixel-Warriors>
#)
