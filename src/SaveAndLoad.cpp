#include "../include/SaveAndLoad.hpp"
#include "../include/globals.hpp"
#include <iostream>

SaveAndLoad::SaveAndLoad() = default;

SaveAndLoad::~SaveAndLoad() = default;
void SaveAndLoad::saveSettings(std::map<std::string, int>& settings) {
    std::cout << "INFO: Saving settings" << std::endl;
    try {
        // Открываем файл для записи
        file.open("./Save/settings.json", std::ios::out);

        if (!file.is_open()) {
            throw std::runtime_error("ERROR: Cannot open file for writing");
        }

        // Очищаем предыдущий JSON объект
        objJson.clear();

        // Создаем JSON объект с настройками
        objJson["settings"] = nlohmann::json::array(); // Массив настроек
        nlohmann::json settingsJson;

        // Добавляем настройки клавиш
        for (const auto& key : keys) {
            std::string settingKey = "keyboard_" + key;
            settingsJson["keys"][key] = settings[settingKey];
        }

        settingsJson["window"]["width"] = settings["windowWidth"];
        settingsJson["window"]["height"] = settings["windowHeight"];
        settingsJson["window"]["fps"] = settings["fps"];
        settingsJson["window"]["fullscreen"] = settings["fullscreen"];
        settingsJson["window"]["V-sync"] = settings["V-sync"];
        settingsJson["squareMove"]["x"] = settings["squareMoveX"];
        settingsJson["squareMove"]["y"] = settings["squareMoveY"];
        settingsJson["mods"] = settings["mods"];


        // Добавляем собранный объект в массив
        objJson["settings"].push_back(settingsJson);

        // Записываем JSON в файл
        file << objJson.dump(4); // Используем отступ в 4 пробела для форматирования
        file.close();

        std::cout << "INFO: Settings saved successfully" << std::endl;
    } catch (const nlohmann::json::exception& error) {
        std::cerr << "ERROR: Failed to save settings: " << error.what() << std::endl;
    } catch (const std::runtime_error& error) {
        std::cerr << "ERROR: " << error.what() << std::endl;
    }
}


std::map<std::string, int> SaveAndLoad::loadSettings() {

    std::cout << "INFO: Loading settings" << std::endl;

    std::map<std::string, int> settings;

    try {
        file.open("./Save/settings.json");
        file >> objJson;
        file.close();

        // Получаем массив настроек
        auto layers = objJson["settings"];

        if (!layers.is_array() || layers.empty()) {
            throw std::runtime_error("ERROR: Invalid settings format");
        }

        // Доступ к первому элементу массива
        auto settingsLayer = layers[0];

        settings["windowWidth"] = settingsLayer["window"]["width"];
        settings["windowHeight"] = settingsLayer["window"]["height"];
        settings["fps"] = settingsLayer["window"]["fps"];
        settings["squareMoveX"] = settingsLayer["squareMove"]["x"];
        settings["squareMoveY"] = settingsLayer["squareMove"]["y"];
        settings["mods"] = settingsLayer["mods"];
        settings["fullscreen"] = settingsLayer["window"]["fullscreen"];
        settings["V-sync"] = settingsLayer["window"]["V-sync"];

        std::cout << "INFO: Window settings loaded successfully" << std::endl;
        std::cout << "INFO: Loading key settings" << std::endl;

        for (const auto& key : keys) {
            std::string settingKey = "keyboard_" + key;
            settings[settingKey] = settingsLayer["keys"][key];
        }

        std::cout << "INFO: Settings loaded successfully" << std::endl;
    }
    catch (const nlohmann::json::exception& error) {
        std::cerr << "ERROR: Failed to load settings: " << error.what() << std::endl;
    }
    catch (const std::runtime_error& error) {
        std::cerr << "ERROR: " << error.what() << std::endl;
    }

    return settings;
}


/*{
"settings"
:
[
    {
    "window": {
    "width": 1280,
    "height": 800,
    "fps": 100,
    "fullscreen": 1,
    "V-sync": 0
    },
    "keys": {
    "A": 65,
    "B": 66,
    "C": 67,
    "D": 68,
    "E": 69,
    "F": 70,
    "G": 71,
    "H": 72,
    "I": 73,
    "J": 74,
    "K": 75,
    "L": 76,
    "M": 77,
    "N": 78,
    "O": 79,
    "P": 80,
    "Q": 81,
    "R": 82,
    "S": 83,
    "T": 84,
    "U": 85,
    "V": 86,
    "W": 87,
    "X": 88,
    "Y": 89,
    "Z": 90,
    "Num0": 48,
    "Num1": 49,
    "Num2": 50,
    "Num3": 51,
    "Num4": 52,
    "Num5": 53,
    "Num6": 54,
    "Num7": 55,
    "Num8": 56,
    "Num9": 57,
    "Escape": 27,
    "LControl": 162,
    "LShift": 160,
    "LAlt": 164,
    "LSystem": 91,
    "RControl": 163,
    "RShift": 161,
    "RAlt": 165,
    "RSystem": 92,
    "Menu": 93,
    "LBracket": 219,
    "RBracket": 221,
    "Semicolon": 186,
    "Comma": 188,
    "Period": 190,
    "Quote": 222,
    "Slash": 191,
    "Backslash": 220,
    "Tilde": 192,
    "Equal": 187,
    "Hyphen": 189,
    "Space": 32,
    "Enter": 13,
    "Backspace": 8,
    "Tab": 9,
    "PageUp": 33,
    "PageDown": 34,
    "End": 35,
    "Home": 36,
    "Insert": 45,
    "Delete": 46,
    "Add": 107,
    "Subtract": 109,
    "Multiply": 106,
    "Divide": 111,
    "Left": 37,
    "Right": 39,
    "Up": 38,
    "Down": 40,
    "Numpad0": 96,
    "Numpad1": 97,
    "Numpad2": 98,
    "Numpad3": 99,
    "Numpad4": 100,
    "Numpad5": 101,
    "Numpad6": 102,
    "Numpad7": 103,
    "Numpad8": 104,
    "Numpad9": 105,
    "F1": 112,
    "F2": 113,
    "F3": 114,
    "F4": 115,
    "F5": 116,
    "F6": 117,
    "F7": 118,
    "F8": 119,
    "F9": 120,
    "F10": 121,
    "F11": 122,
    "F12": 123,
    "F13": 124,
    "F14": 125,
    "F15": 126,
    "Pause": 19
    },
    "squareMove": {
    "x": 40,
    "y": 40
    },
    "mods": 0
    }
]

}*/
