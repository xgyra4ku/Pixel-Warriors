#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class KeystrokeLogging {
public:
    KeystrokeLogging();
    ~KeystrokeLogging();

    char codeToChar(int code);
    std::string getKeyName(sf::Keyboard::Key key);
private:

};