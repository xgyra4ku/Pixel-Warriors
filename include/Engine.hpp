#pragma once
#include <windows.h>
#include <map>
#include <cstdlib>
#include <ctime>
#include "Map.hpp"
#include "SaveAndLoad.hpp"
#include "Entity.hpp"
#include "Console.hpp"


struct DependencyFunctions {
    void (*initLib)(sf::RenderWindow&);
    void (*menuLib)(sf::RenderWindow&, int&, std::map<std::string, int>&, int&, float&, std::map<std::string, std::string>&);
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

    void vRun();
private:
    Map map;
    Entity player1;
    SaveAndLoad save_and_load_;
    Console* oConsole;

    sf::RenderWindow m_oWindow;
    sf::Vector2f m_oPlayerPos;
    sf::Clock m_oClock;
    sf::Clock m_oFpsClock;
    sf::Texture m_oTexturePlayerList[3];
    sf::Vector2i m_vChunkPos;
    sf::Font m_ftFont;


    //float playerSpeed = 0.08f;
    float m_fPlayerSpeed = 5;
    float m_fOffSetSpeed = 10;
    float m_fTime = 0.0f;
    float m_fFps = 0.0f;
    int m_fFrameCount = 0;
    int m_iMenu;
    int m_iWheelEventMouse{};

    bool m_bOffsetRUN;
    bool m_bCollisionRUN;
    bool m_bPlayerPosRUN;
    std::map<std::string, std::string> m_mpFileWorld;

    std::vector<std::vector<int>> m_vMapGenerated;
    std::vector<Mod> m_vModsList;
    std::vector<std::vector<int>> m_mpChunk;
    std::map<std::string, DependencyFunctions> m_mpDependencyList;
    std::map<std::string, int> m_mpSettings;

    void _events();
    void _timer();
    void _logic();
    void _updateDisplay();
    void _initPlayer(int textureNumPlayer1);
    void _controlKeyboard();
    void _collision();
    void _offset() const;
    void _generateMap(unsigned int seed, int WIDTH, int HEIGHT);
    //std::vector<Mod> loadMods(const std::string& directory);
    void _loadDependency(const std::string& directory);
};
