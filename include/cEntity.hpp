#pragma once
//
// Импорт библиотек и файлов
//
#include <string>
#include "globals.hpp"

struct Rect {
    sf::Vector2f position;
    sf::Vector2f size;
};


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
    void setRectСollision(Rect rect);

    void setHP(unsigned int HPSet);

    // гетеры
    sf::Vector2f getPosition() const;
    sf::Sprite getSprite();
    sf::Vector2f getSize() const;

    Rect getRectСollision() const;

    unsigned int getHP() const;

    std::string getName() const;

    // другие функции
    void draw(sf::RenderWindow& window) const;
    void animate(float time, int dir);
private:
    Rect rectСollision; //rect
    //приватные переменые
    sf::Vector2f size;
    std::string name;
    sf::Vector2f position;
    sf::Texture texture;
    sf::Sprite sprite;
    float currentFrame = 0;
    //скорость анимации
    float animationSpeed = 0.004f;
    unsigned int HP;
};