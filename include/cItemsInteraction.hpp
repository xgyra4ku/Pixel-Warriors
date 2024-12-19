#pragma once
#include <fstream>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <map>
#include <nlohmann/json.hpp>

#include "cInfoConsole.hpp"

/// Определение структуры
struct sItem {
    sf::Texture texture;
    sf::Sprite sprite;
    std::string name;
    int id;
    int countInStack;
};

/// Определение класса
class cItemsInteraction
{
public:
    cItemsInteraction();
    ~cItemsInteraction();
private:
    ///
    /// Установка обектов
    ///
    nlohmann::json oJson;
    std::fstream fileSettingsItems;
    cInfoConsole oCmdInfo;

    std::map<int, sItem> mapItems;

    void loadFromFile(const std::string& path);
    sf::Texture loadTexture(const std::string& path);
};