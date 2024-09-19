#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>

class cInfoConsole {
public:
    cInfoConsole();
    ~cInfoConsole();
    void error(const std::string& strValue) const;
    void warning(const std::string& strValue);
    void info(const std::string& strValue);
private:
    const std::string red = "\033[31m";
    const std::string yellow = "\033[33m";
    const std::string green = "\033[32m";
    const std::string reset = "\033[0m";
};


