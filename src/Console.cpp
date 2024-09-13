#include "../include/Console.hpp"

Console::Console(const sf::Font& font) : m_bReflections(false) {
    m_oInputObj.setFont(font);
    m_oInputObj.setCharacterSize(20);//изеняемый шрифт для разрешений
    m_oInputObj.setColorText(sf::Color::White);
    m_oInputObj.setInput(false);
    m_oInputObj.setColorRect(sf::Color::Black);
    m_oInputObj.setPositionText(sf::Vector2f(0,0));
    m_oInputObj.setPositionRectangleShape(sf::Vector2f(0,0));
}
Console::~Console() = default;

void Console::setReflections(const bool bSet) {
    this->m_bReflections = bSet;
}

bool Console::getReflections() const {
    return m_bReflections;
}

void Console::logic(const float fTime) {
    m_oInputObj.readKey(fTime);
}

void Console::draw(sf::RenderWindow window) const {
    if (m_bReflections) {
        m_oInputObj.draw(window);
    }
    //информация
}


