#pragma once
//
// ОПРЕДЕЛЕНИЯ БИБЛИОТЕК
//
#include <windows.h>
#include <map>
#include <cstdlib>
#include <ctime>
#include "cMap.hpp"
#include "SaveAndLoad.hpp"
#include "cEntity.hpp"
#include "Console.hpp"
#include "cInfoConsole.hpp"
#include "cInventory.hpp"

//
// Структура дополнений
//
struct DependencyFunctions {
    void (*initLib)(sf::RenderWindow&);
    void (*menuLib)(sf::RenderWindow&, int&, std::map<std::string, int>&, int&, float&, std::map<std::string, std::string>&);
};

//
// Структура модов
//
struct Mod {
    HMODULE hModule;
    DependencyFunctions functions;
};

//
// Основной класс
//
class Engine
{
public:
    //
    // опредления констукторов диструкторв глобальных функций
    //
    Engine();
    ~Engine();

    //
    // Определение главной функции
    //
    void vRun();
private:
    ///
    /// Настройки по умолчанию
    ///
    int iDISTANCE_VIEW = 2;
    int iCHUNK_SIZE = 16;
    int iTextureNumPlayer1 = 1;


    //
    // Определения других классов
    //
    cMap map;
    cEntity player1;
    SaveAndLoad save_and_load_;
    Console* oConsole;
    cInfoConsole oCmdInfo;
    cInventory* oInventory;

    //
    // Определения классов sfml
    //
    sf::RenderWindow m_oWindow;
    sf::Vector2f m_oPlayerPos;
    sf::Clock m_oClock;
    sf::Clock m_oFpsClock;
    sf::Texture m_oTexturePlayerList[3];
    sf::Vector2i m_vChunkPos;
    sf::Font m_ftFont;

    sf::Text m_sftTextInfo;

    //
    // Определения float
    //
    float m_fPlayerSpeed = 0.08f; //0.08f standard
    float m_fOffSetSpeed = 1.0f; //1.0f standard
    float m_fTime = 0.0f;
    float m_fFps = 0.0f;

    //
    // Определения int
    //
    int m_fFrameCount = 0;
    int m_iMenu;
    int m_iWheelEventMouse{};

    //
    // Определения bool
    //
    bool m_bOffsetRUN;
    bool m_bCollisionRUN;
    bool m_bPlayerPosRUN;
    bool m_bInfo;
    bool m_bCheats;

    //
    // Определения vector
    //
    std::vector<std::vector<int>> m_vMapGenerated;
    std::vector<Mod> m_vModsList;
    std::vector<std::vector<int>> m_mpChunk;

    //
    // Определения map
    //
    std::map<std::string, std::string> m_mpFileWorld;
    std::map<std::string, DependencyFunctions> m_mpDependencyList;
    std::map<std::string, int> m_mpSettings;
    std::map<std::string, int> m_mpCommandData;

    //
    // Определения приватных функций
    //
    void _events();
    void _timer();
    void _logic();
    void _updateDisplay();
    void _initPlayer(int textureNumPlayer1);
    void _controlKeyboard();
    void _commandExecution();
    void _collision();
    void _offset() const;
    void _loadDependency(const std::string& directory);
    //std::vector<Mod> loadMods(const std::string& directory);
};
