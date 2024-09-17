#include "../include/cEntity.hpp"

#include <cmath>

cEntity::cEntity() = default;

cEntity::~cEntity() = default;

//
// Установка позиции
//
void cEntity::setPosition(const sf::Vector2f positionSet) {
    position = positionSet;
    sprite.setPosition(sf::Vector2f(position.x - g_fOffsetX, position.y - g_fOffsetY));
}

//
// Установка имени
//
void cEntity::setName(const std::string &nameSet) {
    this->name = nameSet;
}

//
// Установка текстур
void cEntity::setTexture(const sf::Texture& textureSet) {
    this->texture = textureSet;
    sprite.setTexture(texture);
}

//
// Установка размера
//
void cEntity::setSize(const sf::Vector2f sizeSet) {
    this->size = sizeSet;
}

//
// Получения позиции
//
sf::Vector2f cEntity::getPosition() const {
    return position;
}

//
// Получения спрайта
//
sf::Sprite cEntity::getSprite() {
    return sprite;
}

//
// Полученяе размера
//
sf::Vector2f cEntity::getSize() const {
    return size;
}

//
// Рисовка игрока
//
void cEntity::draw(sf::RenderWindow& window) const {
	window.draw(sprite);
}

//
// Анимации
//
void cEntity::animate(const float time, const int dir) {
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