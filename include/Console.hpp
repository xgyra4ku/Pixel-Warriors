#pragma once
//
// Потключение библиотек
//
#include <map>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <LibKyraText.h>
#include "cInfoConsole.hpp"

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

    //static void getBuffer(int iValue);

    // рисовка
    void draw(sf::RenderWindow& window) const;
    // чтение
    void read(std::map<std::string, int>& mpData);
private:
    // определения приватных переменых
    std::vector<std::string> m_vecStrBufferCommands;
    //int index = -1;
    //int m_iMaxIndex = 20;
    cInfoConsole oCmdInfo;
    ktx::cInputText m_oInputObj;
    bool m_bReflections;
    std::string m_strInput;
};
