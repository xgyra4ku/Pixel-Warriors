#include "../include/cInventory.hpp"


// Конструктор класса sInventoryOBJ, который инициализирует объект инвентаря
sInventoryOBJ::sInventoryOBJ() {
    // Инициализация быстрых слотов инвентаря
    // iSizeFastSlot - количество быстрых слотов, определенное в классе sInventoryOBJ
    for (int i = 1; i <= iSizeFastSlot; ++i) {
        // mapInventoryFast - карта быстрых слотов, где ключ - индекс слота, а значение - идентификатор предмета в слоте
        // В этом цикле все быстрые слоты инициализируются значением 0, что означает, что они пусты
        mapInventoryFast[i] = 0;
    }

    // Инициализация обычных слотов инвентаря
    // iSizeNormalSlot - количество обычных слотов, определенное в классе sInventoryOBJ
    for (int i = 1; i <= iSizeNormalSlot; ++i) {
        // mapInventoryBase - карта обычных слотов, где ключ - индекс слота, а значение - идентификатор предмета в слоте
        // В этом цикле все обычные слоты инициализируются значением 0, что означает, что они пусты
        mapInventoryBase[i] = 0;
    }
}



std::map<int,int> sInventoryOBJ::getFastSlot() const {
    return mapInventoryFast;
}

std::map<int, int> sInventoryOBJ::getBaseSlot() const {
    return mapInventoryBase;
}


void sInventoryOBJ::setFastSlot(const std::map<int, int> &fastSlot) {
    this->mapInventoryFast = fastSlot;
}
void sInventoryOBJ::setBaseSlot(const std::map<int, int> &normalSlot) {
    this->mapInventoryBase = normalSlot;
}

void sInventoryOBJ::setSlotByIndex(const char typeSlots, const int iIndex, const int iItem) {
    if (typeSlots == 'f') {
        mapInventoryFast[iIndex] = iItem;
    } else if (typeSlots == 'b') {
        mapInventoryBase[iIndex] = iItem;
    }
}


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


