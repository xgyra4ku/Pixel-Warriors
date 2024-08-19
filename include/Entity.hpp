#pragma once

#include <string>
#include "globals.hpp"

class Entity
{
public:
    Entity();
    ~Entity();

    void setPosition(sf::Vector2f positionSet);
    void setName(const std::string &nameSet);
    void setTexture(const sf::Texture& textureSet);
    void setSize(sf::Vector2f sizeSet);

    sf::Vector2f getPosition() const;
    sf::Sprite getSprite();
    sf::Vector2f getSize() const;

    void draw(sf::RenderWindow& window) const;
    void animate(float time, int dir);
private:
    sf::Vector2f size;
    std::string name;
    sf::Vector2f position;
    sf::Texture texture;
    sf::Sprite sprite;
    float currentFrame = 0;
    float animationSpeed = 0.004f;
};