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
    // 1-9 слоты быстрого доступа 10-45 обычный инвентарь
    std::map<unsigned int, unsigned int> mpData;
    const unsigned int iInventSize = 45;
public:
    void init() {
        for (int i = 1; i <= iInventSize; ++i) {
            mpData[i] = 0;
        }
    }
    // Геттер для доступа к вектору (как const ссылка)
    [[nodiscard]] const std::map<unsigned int, unsigned int>& getData() const {
        return mpData;
    }

    // Сеттер для изменения данных (например, добавление нового элемента)
    void setElementByIndexFromDate(const unsigned int iValue, const unsigned int iValue2) {
        if (iValue <= iInventSize) {
            mpData[iValue] = iValue2;
        }
    }

    // Функция для удаления элементов
    void deleteElementByIndexFromDate(const unsigned int iValue) {
        if (iValue <= iInventSize) {
            mpData[iValue] = 0;
        }
    }

    // Пример функции для получения элемента по индексу
    unsigned int getElementByIndexFromDate(const unsigned int iValue) {
        if (iValue <= iInventSize) {
            return mpData[iValue];
        }

        return -1;
    }
};
class cInventory {
public:
    // конструкторы диструкторы
    explicit cInventory(const sf::RenderWindow &sfRwWindow);

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
    sInventoryOBJ sInventory;
    sf::RectangleShape m_sfRsHomeInventoryForm;
    sf::RectangleShape m_sfRsInventoryFormList[45];



    bool m_bInventoryIsOpen;
};
