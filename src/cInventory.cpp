#include "../include/cInventory.hpp"

///
/// @brief Конструктор класса инвентаря
/// @param sfRwWindow Окно рендеринга SFML, используемое для определения размера инвентаря
///
cInventory::cInventory(const sf::RenderWindow& sfRwWindow) : m_bInventoryIsOpen(false) {
    m_sfRsHomeInventoryForm.setSize(sf::Vector2f(
        (static_cast<float>(sfRwWindow.getSize().x)* 50 / 100),
        (static_cast<float>(sfRwWindow.getSize().y)* 50 / 100)));
    m_sfRsHomeInventoryForm.setPosition(sf::Vector2f(
        (static_cast<float>(sfRwWindow.getSize().x)* 25 / 100),
        (static_cast<float>(sfRwWindow.getSize().y)* 25 / 100)));
    m_sfRsHomeInventoryForm.setFillColor(sf::Color(46, 46, 46));
    const auto null = sf::Vector2f(
        (static_cast<float>(sfRwWindow.getSize().x) * 25 / 100),
        (static_cast<float>(sfRwWindow.getSize().y) * 25 / 100));

    constexpr float shiftX = 5;
    constexpr float shiftY = 1;

    sInventory.init();
    float x = 1;
    float y = 2;
    const sf::Vector2f size = {m_sfRsHomeInventoryForm.getSize().x / 9 + shiftX, m_sfRsHomeInventoryForm.getSize().y / 5 + shiftY};
    for (int i = 0; i <= 39; i++) {
        m_sfRsInventoryFormList[i].setSize(sf::Vector2f(
        (static_cast<float>(sfRwWindow.getSize().x)* 5.5 / 100),
        (static_cast<float>(sfRwWindow.getSize().y)* 7.5 / 100)));
        if (x == 9) {
            x = 2;
            y++;
        } else {
            x++;
        }

        m_sfRsInventoryFormList[i].setPosition((x -1) *size.x + m_sfRsHomeInventoryForm.getSize().x/2.39, y*size.y + m_sfRsHomeInventoryForm.getSize().y/7.2);
        m_sfRsInventoryFormList[i].setFillColor(sf::Color(128,128,128));
    }
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
        pWindow.draw(m_sfRsHomeInventoryForm);
        for (int i = 0; i <= 44; i++) {
            pWindow.draw(m_sfRsInventoryFormList[i]);
        }
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


