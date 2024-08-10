#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <nlohmann/json.hpp>
#include "globals.hpp"

class SaveAndLoad {

public:
    void saveSettings(std::map<std::string, int> settings);
    std::map<std::string, int> loadSettings();
private:
    nlohmann::json objJson;
    
    std::fstream file;
};



