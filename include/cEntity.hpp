#pragma once
//
// Импорт библиотек и файлов
//
#include <string>
#include "globals.hpp"

//
// Создание класса
//
class cEntity
{
public:
    // конструкторы диструкторы
    cEntity();
    ~cEntity();

    // сетеры
    void setPosition(sf::Vector2f positionSet);
    void setName(const std::string &nameSet);
    void setTexture(const sf::Texture& textureSet);
    void setSize(sf::Vector2f sizeSet);

    // гетеры
    sf::Vector2f getPosition() const;
    sf::Sprite getSprite();
    sf::Vector2f getSize() const;

    // другие функции
    void draw(sf::RenderWindow& window) const;
    void animate(float time, int dir);
private:
    //приватные переменые
    sf::Vector2f size;
    std::string name;
    sf::Vector2f position;
    sf::Texture texture;
    sf::Sprite sprite;
    float currentFrame = 0;
    //скорость анимации
    float animationSpeed = 0.004f;
};