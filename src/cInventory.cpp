#include "../include/cInventory.hpp"

///
/// @brief Конструктор класса инвентаря
/// @param texture Изображение инвентаря
/// @param sfRwWindow Окно рендеринга SFML
///
cInventory::cInventory(const sf::Texture &texture, const sf::RenderWindow &sfRwWindow) : m_bInventoryIsOpen(false) {
    m_texture = texture;  // Копируем текстуру
    spriteInventory.setTexture(m_texture);
    // Целевые размеры
    const float targetWidth = (static_cast<float>(sfRwWindow.getSize().x)* 50 / 100);
    const float targetHeight = (static_cast<float>(sfRwWindow.getSize().x)* 30 / 100);

    // Получаем размеры текстуры
    const sf::Vector2u textureSize = texture.getSize();

    // Вычисляем масштабирование
    const float scaleX = targetWidth / textureSize.x;
    const float scaleY = targetHeight / textureSize.y;

    // Устанавливаем масштаб
    spriteInventory.setScale(scaleX, scaleY);
    spriteInventory.setPosition(
        (static_cast<float>(sfRwWindow.getSize().x)* 25 / 100),
        (static_cast<float>(sfRwWindow.getSize().y)* 25 / 100));
}

///
/// @brief Диструктор класса инвентаря
///
cInventory::~cInventory() = default;

///
/// @brief Отрисовка инвентаря
/// @param pWindow  Окно рендеринга SFML, используемое для отрисовки инвентаря
///
void cInventory::draw(sf::RenderWindow& pWindow) const {
    if (m_bInventoryIsOpen) {
        pWindow.draw(spriteInventory);
    }
}


///
/// @brief Проверяет наличие предмета в инвентаре
/// @param iIndexItem индекс предмена
/// @return да если есть предмет в инвентаре
bool cInventory::checkIfTheItemIsInInventoryByIndex(int iIndexItem) {
    return false;
}

///
/// @brief Сеттер статуса инвентаря
/// @param bValue Определяет показываеться или нет инвентарь
///
void cInventory::setInventoryStatus(const bool bValue) {
    m_bInventoryIsOpen = bValue;
}

///
/// @brief Сеттер элемента по индексу
/// @param iIndex Индекс по инвентарю
/// @param iItem Элемент его индекс/id
///
void cInventory::setItemByIndex(const unsigned int iIndex, const unsigned int iItem) {

}

///
/// @brief Геттер стуса инвентаря
/// @return Возращает статус инвентаря
///
bool cInventory::getInventoryStatus() const {
    return m_bInventoryIsOpen;
}

///
/// @brief Геттер возращает индекс элемента по индексу в инвентаря
/// @param iIndexItem Идекс по инвентарю
/// @return Индекс элемента
///
int cInventory::getIdItemByIndex(int iIndexItem) const {
    return -1;
}


