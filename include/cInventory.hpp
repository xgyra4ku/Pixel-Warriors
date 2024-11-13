#pragma once
#include <vector>
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>

//
// Структура инвентаря
//
struct sInventoryOBJ {
private:
    std::map<int, int> mapInventoryBase; // 4x6
    std::map<int, int> mapInventoryFast; // 1x9
    std::map<int, int> mapInventoryProtection; // 2x2
    // слоты инвентаря
    std::map<unsigned int, unsigned int> fastSlot;
    std::map<unsigned int, unsigned int> normalSlot;
    // размеры слотов
    int iSizeFastSlot = 10;
    int iSizeNormalSlot = 24;
public:
    sInventoryOBJ();


    // гетеры
    [[nodiscard]] std::map<unsigned int, unsigned int> getFastSlot() const;
    [[nodiscard]] std::map<unsigned int, unsigned int> getNormalSlot() const;
    // сетеры
    void setFastSlot(const std::map<unsigned int, unsigned int> &fastSlot);
    void setNormalSlot(const std::map<unsigned int, unsigned int> &normalSlot);
    // инные функции
    // ...

};
class cInventory {
public:
    // конструкторы диструкторы
    explicit cInventory(const sf::RenderWindow &sfRwWindow);

    cInventory(const sf::Texture &texture);

    cInventory(const sf::Texture &texture, const sf::RenderWindow &sfRwWindow);

    ~cInventory();

    // гетеры
    [[nodiscard]] int getIdItemByIndex(int iIndexItem) const;
    [[nodiscard]] bool getInventoryStatus() const;

    // сетеры
    void setItemByIndex(unsigned int iIndex, unsigned int iItem);
    void setInventoryStatus(bool bValue);

    // инные функции
    void draw(sf::RenderWindow &pWindow) const;
    bool checkIfTheItemIsInInventoryByIndex(int iIndexItem);
private:
    sf::Texture m_texture;  // Хранить текстуру внутри объекта
    sInventoryOBJ sInventory;
    sf::Sprite spriteInventory;

    bool m_bInventoryIsOpen;
};
