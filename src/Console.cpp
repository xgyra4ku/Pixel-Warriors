#include "../include/Console.hpp"
#include <sstream>
#include <iostream>

//
// Конструктор консоли
// Иницилизация переменых
// Определения библиотеки ввода
//
Console::Console(const sf::Font& font, const sf::RenderWindow &window) : m_bReflections(false) {
    m_oInputObj.setFont(font);
    m_oInputObj.setCharacterSize(20);
    m_oInputObj.setColorText(sf::Color::White);
    m_oInputObj.setInput(false);
    m_oInputObj.setColorRect(sf::Color::Black);
    m_oInputObj.setPositionText(sf::Vector2f(0,0));
    m_oInputObj.setPositionRectangleShape(sf::Vector2f(0,0));
    m_oInputObj.setRectSize(sf::Vector2f(static_cast<float>(window.getSize().x), 30));
}

//
// Диструктор
//
Console::~Console() = default;

//
// Сетер для вкл/отк отображения консоли
//
void Console::setReflections(const bool bSet) {
    this->m_bReflections = bSet;
    m_oInputObj.setInput(m_bReflections);
}

//
// Гетер отображения консоли
//
bool Console::getReflections() const {
    return m_bReflections;
}

//
// Логика консоли и передача аргумента времения для синхронизации
//
void Console::logic(const float fTime) {
    m_oInputObj.readKey(fTime);
}

//
// Чтения и парсинг команд выполнение
//
void Console::read(std::map<std::string, int>& mpData) {
    std::istringstream iss(m_oInputObj.getString());
    mpData.clear();
    // Извлечение первой части строки как команды
    if (std::string command; iss >> command) {
        // Если команда не пустая
        if (std::string parameter; iss >> parameter) {
            // Если параметр тоже не пустой
            mpData[command] = std::stoi(parameter);
            std::cout << "INFO: Command: " << command << std::endl;
            std::cout << "INFO: Parameter: " << parameter << std::endl;
        } else {
            std::cout << "INFO: Command: " << command << std::endl;
            std::cout << "INFO: No parameter provided." << std::endl;
        }
    } else {
        std::cout << "INFO: No command provided." << std::endl;
    }
    m_oInputObj.setString("");
}

//
// Отрисовка консоли если доступно
//
void Console::draw(sf::RenderWindow& window) const {
    if (m_bReflections) {
        m_oInputObj.draw(window);
    }
    //информация
}


