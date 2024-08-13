#pragma once
#include <windows.h>
#include <map>
#include "globals.hpp"
#include "Map.hpp"
#include "SaveAndLoad.hpp"
#include "Entity.hpp"

struct DependencyFunctions {
    void (*initLib)(sf::RenderWindow&);
    void (*menuLib)(sf::RenderWindow&, int&, std::map<std::string, int>&);
};

struct Mod {
    HMODULE hModule;
    DependencyFunctions functions;
};

class Engine
{
    public:
        Engine();
        ~Engine();

        void run();
    private:
        Map map;
        Entity player1;
        SaveAndLoad save_and_load_;


        sf::RenderWindow window;
        sf::Vector2f playerPos;
        sf::Clock clock;
        sf::Clock fpsClock;
        sf::Texture texturePlayerList[3];

        float playerSpeed = 0.08f;
        float time = 0.0f;
        float fps = 0.0f;
        int frameCount = 0;
        int menu;

        bool offsetRUN;
        bool collisionRUN;

        std::vector<std::vector<int>> mapGenerated;
        std::vector<Mod> modsList;
        std::map<std::string, DependencyFunctions> dependencyList;
        std::map<std::string, int> settings;

        void Events();
        void timer();
        void logic();
        void updateDisplay();
        void initPlayer(int textureNumPlayer1);
        void controlKeyboard();
        void collision();
        void offset();
        void generateMap(unsigned int seed, int WIDTH, int HEIGHT);
        //std::vector<Mod> loadMods(const std::string& directory);
        void loadDependency(const std::string& directory);
};
