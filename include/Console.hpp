#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Console {
public:
    Console();
    ~Console();
private:
    std::vector<sf::Keyboard::Key> m_viListKeys;
};
