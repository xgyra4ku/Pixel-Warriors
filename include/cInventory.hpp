#pragma once
#include <vector>
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>

//
// Структурав инвентаря
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
    void draw(sf::RenderWindow window);
    void getIdItemByIndex(int iIndexItem);

    bool setItemByIndex();

    bool checkIfTheItemIsInInventoryByIndex(int iIndexItem);
private:
};