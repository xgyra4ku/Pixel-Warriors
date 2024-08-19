#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class KeystrokeLogging {
public:
    static char codeToChar(int code);

    static std::string getKeyName(sf::Keyboard::Key key);
};