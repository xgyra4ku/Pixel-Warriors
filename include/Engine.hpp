#pragma once
#include "globals.hpp"
#include "Map.hpp"
#include "Entity.hpp"

class Engine
{
    public:
        Engine();
        ~Engine();

        void run();
    private:
        Map map;
        Entity player1;

        sf::RenderWindow window;
        sf::Vector2f playerPos;
        sf::Clock clock;
        sf::Clock fpsClock;

    
        sf::Texture texturePlayerList[3];
        
        float playerSpeed = 0.08f;
        float time = 0.0f;
        int frameCount = 0;
        float fps = 0.0f;
        
        void Events();
        void logic();
        void updateDisplay();
        void initPlayer(int textureNumPlayer1);
        void controlKeyboard();
        void collision();
};
