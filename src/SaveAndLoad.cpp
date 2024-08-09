#include "../include/SaveAndLoad.hpp"

void SaveAndLoad::saveSettings(std::map<std::string, int> settings) {
    std::cout << "INFO: Saving settings" << std::endl;
    try {
        file.open("./Save/settings.json");
        objJson["settings"][0]["window"]["width"] = settings["windowWidth"];
        objJson["settings"][0]["window"]["height"] = settings["windowHeight"];
        objJson["settings"][0]["window"]["fps"] = settings["fps"];
        objJson["settings"][0]["squareMove"]["x"] = settings["squareMoveX"];
        objJson["settings"][0]["squareMove"]["y"] = settings["squareMoveY"];
        objJson["settings"][0]["mods"] = settings["mods"];
        file << objJson;
        file.close();
        std::cout << "INFO: Settings saved successfully" << std::endl;
    }
    catch (const nlohmann::json::exception& error) {
        std::cerr << "ERROR: Failed to save settings" << error.what() << std::endl;
    }

}

std::map<std::string, int> SaveAndLoad::loadSettings() {

    std::cout << "INFO: Loading settings" << std::endl;

    std::map<std::string, int> settings;

    try {
        file.open("./Save/settings.json");
        file >> objJson;
        file.close();
        auto layers = objJson["settings"];
        WindowWidth = layers[0]["window"]["width"];
        WindowHeight = layers[0]["window"]["height"];

        settings["windowWidth"] = layers[0]["window"]["width"];
        settings["windowHeight"] = layers[0]["window"]["height"];
        settings["fps"] = layers[0]["window"]["fps"];
        settings["squareMoveX"] = layers[0]["squareMove"]["x"];
        settings["squareMoveY"] = layers[0]["squareMove"]["y"];
        settings["mods"] = layers[0]["mods"];
        std::cout << "INFO: Settings loaded successfully" << std::endl;
    }
    catch (const nlohmann::json::exception& error) {
        std::cerr << "ERROR: Failed to load settings" << error.what() << std::endl;
    }

    return settings;
}