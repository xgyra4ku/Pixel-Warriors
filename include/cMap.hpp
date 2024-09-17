#pragma once
//
// Определение функций и файлов
//
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <map>
#include <utility>
#include <thread>
#include <mutex>

#include "nlohmann/json.hpp"
#include "globals.hpp"
#include "PerlinNoise.hpp"

//
// структура ЧАНКА
//
struct chunk {
    std::vector<std::vector<int>> data;
    sf::Vector2f pos;
};

//
// Определение класса
//
class cMap
{
public:
    // конструкторы диструкторы
    cMap();
    ~cMap();

    // удаление старых данных для выхода или загрузки нового мира
    void deleting();


    void load();// загрузка из файла
    void save();//сохранение
    void init(int iDistanceView, const std::string &strNameFileMap, const sf::RenderWindow &window);// иницилизация уже до этого созданого мира
    void init(int iDistanceView, const std::string& strNameFileMap, const sf::RenderWindow& window, unsigned int seed);// иницилизация нового мира
    void draw(sf::RenderWindow &window, sf::Vector2f playerPos, sf::Vector2f view, int chunkSize); // рисовка
    void draw(sf::RenderWindow &window, const std::vector<std::vector<int>>& Layer, sf::Vector2f playerPos, sf::Vector2f viev) const;// рисовка
    int collision(sf::Vector2f playerPos, sf::Vector2f playerSize, sf::Vector2f bias) const;// коллизии
    void setLayer(int x, int y, int layer, int value);// установка слоя
    int getLayer(int x, int y, int layer) const;// получение слоя
    sf::Vector2f getPosPlayer(); // получение позиции игрока из загруженого мира
    void createWorld(); //создание мира

    // клеточный автомат
    static void initializeMap(std::vector<std::vector<int>>& map, unsigned int seed, double INITIAL_PROB, int WIDTH, int HEIGHT);
    static void stepAutomaton(std::vector<std::vector<int>>& map, int WIDTH, int HEIGHT);
private:
    // переменые sfml
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f PlayerPos;

    // установка обектов
    using json = nlohmann::json;
    using unorderedMapChunks = std::unordered_map<std::string, std::vector<std::vector<int>>>;

    // определение json
    json objJson;
    json jsonSave;
    json jsonLoad;

    // определения буферов
    unorderedMapChunks chunkBuffer;
    unorderedMapChunks chunkBufferLoadIsFile;

    // определения записи и сохранения обектов файла
    std::fstream fileInput;
    std::fstream fileWorld;

    // определения натсроек текстура и прорисовки
    int imageHeight{};
    int imageWidth{};
    int m_iDistanceView;

    bool ChunksThreadOnOff;
    bool fileWorldIsOpen;
    bool m_bNewWorld;

    unsigned int m_uiSeed{};

    int LayerOdj[g_LayerSizeMaxX][g_LayerSizeMaxY]{};
    int LayerGround[g_LayerSizeMaxX][g_LayerSizeMaxY]{};

    // старые чанки
    std::map<std::pair<int, int>, std::vector<std::vector<int>>> chunks;

    //векторт чанков
    std::vector<chunk> chunkVector;

    // потоки и мютексы
    std::thread chunkLoadThread;
    std::mutex chunkMutex;

    std::string strNameFileWorld;

    //
    // Приватные функции
    //
    void funkLoadChunksThread();
    void startChunkLoadingThread();
    void stopChunkLoadingThread();

    void loadingChunksFromFile();

    bool checkingDownloadedChunks(const std::string& requiredChunk, std::vector<std::vector<int>> &chunkData);

    void loadChunksAroundPlayer(sf::Vector2f playerPos, int chunkSize);
    void unloadDistantChunks(sf::Vector2f playerPos, int chunkSize);
    void saveChunk(int chunkX, int chunkY, const std::vector<std::vector<int>> &data, int chunkSize);

    static void chunkAdaptation(const std::vector<std::vector<double>> &noiseValues, std::vector<std::vector<int>> &chunk, int chunkSize);
    static void generateRivers(std::vector<std::vector<int>>& chunk, int chunkSize);
    static double generatePerlinNoise(double x, double y, double scale, int octaves, double persistence, unsigned int seed);
    static std::vector<std::vector<int>> generateChunk(int chunkX, int chunkY, unsigned int seed, int chunkSize);
};
