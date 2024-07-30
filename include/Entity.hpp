#pragma onse
#include <string>
#include "globals.hpp"

class Entity
{
public:
    Entity();
    ~Entity();

    virtual void setPosition(sf::Vector2f positionSet);
    virtual void setName(std::string nameSet);
    virtual void setTexture(sf::Texture textureSet);

    virtual sf::Vector2f getPosition();
    virtual sf::Sprite getSprite();

    virtual void draw(sf::RenderWindow& window);

    std::string name;
    sf::Vector2f position;
    sf::Texture texture;
    sf::Sprite sprite;
private:
    
};