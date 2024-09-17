#pragma once
//
// определение файлов и библиотек
//
#include <iostream>
#include <SFML/Graphics.hpp>

//
// Определение класса
//
class KeystrokeLogging {
public:
    //статические функции
    static char codeToChar(int code);
    static std::string getKeyName(sf::Keyboard::Key key);
};