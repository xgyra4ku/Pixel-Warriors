#include "../include/Entity.hpp"

Entity::Entity() {
}

Entity::~Entity() {
}

void Entity::setPosition(sf::Vector2f positionSet) {
    position = positionSet;
    sprite.setPosition(position);
}

void Entity::setName(std::string nameSet) {
    this->name = nameSet;
}
void Entity::setTexture(sf::Texture textureSet) {
    this->texture = textureSet;
    sprite.setTexture(texture);
}

sf::Vector2f Entity::getPosition() {
    return position;
}
sf::Sprite Entity::getSprite() {
    return sprite;
}
void Entity::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}