#include "../include/cInventory.hpp"

///
/// @brief Конструктор класса инвентаря
/// @param sfRwWindow Окно рендеринга SFML, используемое для определения размера инвентаря
///
cInventory::cInventory(sf::RenderWindow sfRwWindow) : m_bInventoryIsOpen(false) {
    m_sfRsHomeInventoryForm.setSize(sf::Vector2f(
        (static_cast<float>(sfRwWindow.getSize().x)* 50 / 100),
        (static_cast<float>(sfRwWindow.getSize().y)* 50 / 100)));
}

///
/// @brief Диструктор класса инвентаря
///
cInventory::~cInventory() = default;

///
/// @brief Отрисовка инвентаря
/// @param sfRwWindow  Окно рендеринга SFML, используемое для отрисовки инвентаря
///
void cInventory::draw(sf::RenderWindow sfRwWindow) const {
    if (m_bInventoryIsOpen) {
        //window.draw();
    }
}

bool cInventory::checkIfTheItemIsInInventoryByIndex(int iIndexItem) {

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

}


