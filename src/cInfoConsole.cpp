#include "../include/cInfoConsole.hpp"

cInfoConsole::cInfoConsole() = default;
cInfoConsole::~cInfoConsole() = default;

void cInfoConsole::error(const std::string& strValue) const {
    // Получаем текущее время
    const auto now = std::chrono::system_clock::now();
    const std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Преобразуем в строку
    const std::tm* local_time = std::localtime(&now_time);

    std::cout << red << '[' << std::put_time(local_time, "%H:%M:%S") << "ERROR]:" << strValue << reset << std::endl;
}


