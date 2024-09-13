#pragma once
//
// Потключение библиотек
//
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <LibKyraText.h>

class Console {
public:
    // определение конструкторов диструкторов
    Console(const sf::Font &font, const sf::RenderWindow &window);
    ~Console();

    // определение сетеров и гетеров
    void setReflections(bool bSet);
    bool getReflections() const;

    // определения других функция
    //  логика
    void logic(float fTime);
    // рисовка
    void draw(sf::RenderWindow& window) const;
    // чтение
    void read(std::map<std::string, int>& mpData);
private:
    // определения приватных переменых
    ktx::cInputText m_oInputObj;
    bool m_bReflections;
    std::string m_strInput;
};
