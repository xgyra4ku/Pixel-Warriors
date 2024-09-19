#pragma once
//
// Определение библиотек
//
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>

//
// создание класса
//
class cInfoConsole {
public:
    cInfoConsole();
    ~cInfoConsole();

    void error(const std::string& strValue);
    void warning(const std::string &strValue);
    void info(const std::string &strValue);

    static void enableANSI();
private:
    //структура времени
    std::tm* local_time{};

    //цвета для окрашивания консоли
    const std::string red = "\033[31m";
    const std::string yellow = "\033[33m";
    const std::string green = "\033[32m";
    const std::string reset = "\033[0m";

    void updateTime();
};


