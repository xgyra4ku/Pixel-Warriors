#pragma once
#include "globals.hpp"
#include "Map.hpp"
#include "Player.hpp"

class Engine
{
    public:
        Engine();
        ~Engine();

        void run();
    private:
        Map map;
        Entity player1;
        Player player2;

        sf::RenderWindow window;
        sf::Vector2f playerPos;

        sf::Texture texturePlayerList[3];
        
        float plaerSpeed = 5;
        
        void Events();
        void logic();
        void updateDisplay();
        void initPlayer(int textureNumPlayer1);
        void controlKeyboard();
};
