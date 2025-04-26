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
#include "cInfoConsole.hpp"

//
// структура ЧАНКА
//
struct ChunkStruct {
    std::vector<std::vector<int>> data;
    sf::Vector2f pos;
    std::vector<std::vector<int>> dataObjects;
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

    void save();//сохранение
    void init(int iDistanceView, const std::string &strNameFileMap, const sf::RenderWindow &window);// иницилизация уже до этого созданого мира

    void init(int iDistanceView, const std::string& strNameFileMap, const sf::RenderWindow& window, unsigned int seed);// иницилизация нового мира
    void initAndStartThreads();

    void draw(sf::RenderWindow &window, sf::Vector2f playerPos, sf::Vector2f view, int chunkSize); // рисовка
    void swapBuffers();

    void threadFunction();

    int collision(sf::Vector2f playerPos, sf::Vector2f playerSize, sf::Vector2f bias) const;// коллизии
    void setLayer(int x, int y, int layer, int value);// установка слоя
    int getLayer(int x, int y, int layer) const;// получение слоя
    sf::Vector2f getPosPlayer(); // получение позиции игрока из загруженого мира
    void createWorld(); //создание мира
private:
    // определение инных классов
    cInfoConsole oCmdInfo;

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

    std::mutex MUTEX;
    std::mutex MUTEX2;
    sf::Thread thread; // Поток как член класса
    sf::Thread thread2; // Поток как член класса

    // определения натсроек текстура и прорисовки
    int imageHeight{};
    int imageWidth{};
    int m_iDistanceView;

    bool ChunksThreadOnOff{};
    bool fileWorldIsOpen;
    bool m_bNewWorld;
    bool isThreadRunning;
    bool isThreadStarted;

    unsigned int m_uiSeed{};

    int LayerOdj[g_LayerSizeMaxX][g_LayerSizeMaxY]{};
    int LayerGround[g_LayerSizeMaxX][g_LayerSizeMaxY]{};

    // старые чанки
    std::map<std::pair<int, int>, ChunkStruct> chunks;
    std::map<std::pair<int, int>, ChunkStruct> chunksRenderBuffer;
    //std::map<std::pair<int, int>, ChunkStruct> chunksWorkBuffer;
    std::mutex bufferMutex;


    //векторт чанков
    std::vector<ChunkStruct> chunkVector;
    sf::VertexArray vertices;
    sf::VertexArray verticesTree;

    std::string strNameFileWorld;

    const int chunkSize = 16;

    void loadTileSet();
    void chunkLoadUnloadThread();
    void loadingChunksFromFile();
    void loadChunksAroundPlayer();
    void unloadDistantChunks();
    void saveChunk(int chunkX, int chunkY, const std::vector<std::vector<int>> &data, int chunkSize);

    bool checkingDownloadedChunks(const std::string& requiredChunk, std::vector<std::vector<int>> &chunkData);


    static void chunkAdaptation(const std::vector<std::vector<double>> &noiseValues, std::vector<std::vector<int>> &chunk, int chunkSize);
    static void generateRivers(std::vector<std::vector<int>>& chunk, int chunkSize);

    static double generatePerlinNoise(double x, double y, double scale, int octaves, double persistence, unsigned int seed);

    static ChunkStruct generateChunk(int chunkX, int chunkY, unsigned int seed, int chunkSize);
};
