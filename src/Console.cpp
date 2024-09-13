#include "../include/Console.hpp"


//
// Конструктор консоли
// Определения библиотеки ввода
// Иницилизация переменых
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
void Console::read() {
    m_strInput = m_oInputObj.getString();
    // написания парсера и чтения команд
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


