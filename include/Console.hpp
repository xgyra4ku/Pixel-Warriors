#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <LibKyraText.h>

class Console {
public:
    Console();
    void logic(float fTime);
    void setReflections(bool bSet);
    bool getReflections() const;
    void draw(sf::RenderWindow window) const;

    Console(const sf::Font &font);

    ~Console();
private:
    ktx::cInputText m_oInputObj;

    bool m_bReflections;
};
