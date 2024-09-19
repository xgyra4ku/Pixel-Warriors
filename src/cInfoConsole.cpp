#include "../include/cInfoConsole.hpp"
#include <windows.h>

cInfoConsole::cInfoConsole() = default;
cInfoConsole::~cInfoConsole() = default;

//
// Вывод ошибки
//
void cInfoConsole::error(const std::string& strValue) {
    updateTime();
    std::cout << red << '[' << std::put_time(local_time, "%H:%M:%S") << "][" << "ERROR]: " << strValue << reset << std::endl;
}

//
// Вывод предупреждения
//
void cInfoConsole::warning(const std::string& strValue) {
    updateTime();
    std::cout << yellow << '[' << std::put_time(local_time, "%H:%M:%S") << "][" << "WARNING]: " << strValue << reset << std::endl;
}

//
// Вывод информации
//
void cInfoConsole::info(const std::string& strValue) {
    updateTime();
    std::cout << green << '[' << std::put_time(local_time, "%H:%M:%S") << "][" << "INFO]: " << strValue << reset << std::endl;
}

//
// Обновления времени
//
void cInfoConsole::updateTime() {
    // Получаем текущее время
    const auto now = std::chrono::system_clock::now();
    const std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Преобразуем в строку
    local_time = std::localtime(&now_time);
}

//
// Для окрашивания консоли
//
void cInfoConsole::enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}




