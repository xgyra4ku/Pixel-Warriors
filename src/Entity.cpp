#include "../include/Entity.hpp"

#include <cmath>

Entity::Entity() = default;

Entity::~Entity() = default;

void Entity::setPosition(const sf::Vector2f positionSet) {
    position = positionSet;
    sprite.setPosition(sf::Vector2f(position.x - g_fOffsetX, position.y - g_fOffsetY));
}

void Entity::setName(const std::string &nameSet) {
    this->name = nameSet;
}
void Entity::setTexture(const sf::Texture& textureSet) {
    this->texture = textureSet;
    sprite.setTexture(texture);
}

void Entity::setSize(const sf::Vector2f sizeSet) {
    this->size = sizeSet;
}

sf::Vector2f Entity::getPosition() const {
    return position;
}
sf::Sprite Entity::getSprite() {
    return sprite;
}

sf::Vector2f Entity::getSize() const {
    return size;
}
void Entity::draw(sf::RenderWindow& window) const {
	window.draw(sprite);
}
void Entity::animate(const float time, const int dir) {
    if (dir == 1)
	{
		currentFrame += animationSpeed * time;
		if (currentFrame > 4) currentFrame -= 4;
		sprite.setTextureRect(sf::IntRect(size.x * int(currentFrame), 80, size.x, size.y));
	}
    else if (dir == 2)
	{
		currentFrame += animationSpeed * time;
		if (currentFrame > 4) currentFrame -= 4;
		sprite.setTextureRect(sf::IntRect(size.x * int(currentFrame) + size.x, 64, -size.x, size.y));
	}
    else if (dir == 3)
	{
		currentFrame += animationSpeed * time;
		if (currentFrame > 4) currentFrame -= 4;
		sprite.setTextureRect(sf::IntRect(size.x * int(currentFrame), 48, size.x, size.y));
	}
    else if (dir == 4)
	{
		currentFrame += animationSpeed * time;
		if (currentFrame > 4) currentFrame -= 4;
		sprite.setTextureRect(sf::IntRect(size.x * int(currentFrame), 64, size.x, size.y));
	}
    else if (dir == 5)
	{
		currentFrame += animationSpeed * time;
		if (currentFrame > 2) currentFrame -= 2;
		sprite.setTextureRect(sf::IntRect(size.x * int(currentFrame), 0, size.x, size.y));
	}
}