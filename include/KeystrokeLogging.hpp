#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class KeystrokeLogging {
public:
    char codeToChar(int code);
    std::string getKeyName(sf::Keyboard::Key key);
private:

};