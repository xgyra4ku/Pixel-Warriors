#include "../include/cItemsInteraction.hpp"

#include "../include/cInfoConsole.hpp"

cItemsInteraction::cItemsInteraction() = default;

cItemsInteraction::~cItemsInteraction() = default;

void cItemsInteraction::loadFromFile(const std::string& path) {
    std::cout << "INFO: Open file " << path << std::endl;
    std::ifstream fileSettingsItems(path);
    if (!fileSettingsItems.is_open()) {
        std::cerr << "ERROR: Failed to open file: " << path << std::endl;
        return;
    }

    try {
        fileSettingsItems >> oJson;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: Failed to parse JSON: " << e.what() << std::endl;
        fileSettingsItems.close();
        return;
    }
    fileSettingsItems.close();

    // Перебираем элементы JSON
    for (auto& [key, value] : oJson.items()) {
        if (value.contains("texture") && value["texture"].is_string()
            && value.contains("name") && value["name"].is_string()
            && value.contains("countStack") && value["countStack"].is_number_integer()) {
            sItem item;
            std::string texturePath = value["texture"];
            item.texture = loadTexture(texturePath);
            item.sprite.setTexture(item.texture);
            item.name = value["name"];
            item.id = std::stoi(key);
            item.countInStack = value["countStack"];
            mapItems[item.id] = item;
        } else {
            oCmdInfo.error("Invalid JSON format for item " + key);
        }
    }
}

sf::Texture cItemsInteraction::loadTexture(const std::string& path) {
    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        oCmdInfo.error("Failed to load texture: " + path);
    }
    return texture;
}

//items.json
//{
//    "1": {
//        "texture": "texture.png",
//        "name": "name",
//        "countStack": 1
//    },
//    "2": {
//        "texture": "texture.png",
//        "name": "name",
//        "countStack": 1
//    }
//}

