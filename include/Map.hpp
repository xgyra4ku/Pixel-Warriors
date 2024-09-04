#pragma once
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

struct chunk {
    std::vector<std::vector<int>> data;
    sf::Vector2f pos;
};


class Map
{
public:
    Map();
    ~Map();

    void deleting() const;

    void load();
    void save();
    void init(int iDistanceView, const std::string &strNameFileMap, const sf::RenderWindow &window);
    void draw(sf::RenderWindow &window, sf::Vector2f playerPos, sf::Vector2f view, int chunkSize);
    void draw(sf::RenderWindow &window, const std::vector<std::vector<int>>& Layer, sf::Vector2f playerPos, sf::Vector2f viev) const;

    int collision(sf::Vector2f playerPos, sf::Vector2f playerSize, sf::Vector2f bias) const;

    void setLayer(int x, int y, int layer, int value);

    int getLayer(int x, int y, int layer) const;
    sf::Vector2f getPosPlayer();

    void createWorld();


    static void initializeMap(std::vector<std::vector<int>>& map, unsigned int seed, double INITIAL_PROB, int WIDTH, int HEIGHT);
    static void stepAutomaton(std::vector<std::vector<int>>& map, int WIDTH, int HEIGHT);
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f PlayerPos;

    using json = nlohmann::json;
    using unorderedMapChunks = std::unordered_map<std::string, std::vector<std::vector<int>>>;

    json objJson;
    json jsonSave;
    json jsonLoad;

    unorderedMapChunks chunkBuffer;
    unorderedMapChunks chunkBufferLoadIsFile;

    std::fstream fileInput;
    std::fstream fileWorld;

    int imageHeight{};
    int imageWidth{};
    int m_iDistanceView;

    bool ChunksThreadOnOff;
    bool fileWorldIsOpen;

    unsigned int m_uiSeed{};

    int LayerOdj[g_LayerSizeMaxX][g_LayerSizeMaxY]{};
    int LayerGround[g_LayerSizeMaxX][g_LayerSizeMaxY]{};

    std::map<std::pair<int, int>, std::vector<std::vector<int>>> chunks;

    std::vector<chunk> chunkVector;

    std::thread chunkLoadThread;
    std::mutex chunkMutex;

    std::string strNameFileWorld;



    void funkLoadChunksThread();
    void startChunkLoadingThread();
    void stopChunkLoadingThread();

    void loadingChunksFromFile(const std::string &nameFile);

    bool checkingDownloadedChunks(const std::string& requiredChunk, std::vector<std::vector<int>> &chunkData);

    void loadChunksAroundPlayer(sf::Vector2f playerPos, int chunkSize);
    void unloadDistantChunks(sf::Vector2f playerPos, int chunkSize);
    void saveChunk(int chunkX, int chunkY, const std::vector<std::vector<int>> &data, int chunkSize);

    static void chunkAdaptation(const std::vector<std::vector<double>> &noiseValues, std::vector<std::vector<int>> &chunk, int chunkSize);
    static void generateRivers(std::vector<std::vector<int>>& chunk, int chunkSize);
    static double generatePerlinNoise(double x, double y, double scale, int octaves, double persistence, unsigned int seed);
    static std::vector<std::vector<int>> generateChunk(int chunkX, int chunkY, unsigned int seed, int chunkSize);
};
