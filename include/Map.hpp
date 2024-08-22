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

    void init(int distanceView);
    void load();

    static void save();
    void draw(sf::RenderWindow &window, const std::vector<std::vector<int>>& Layer, sf::Vector2f playerPos, sf::Vector2f viev) const;
    int collision(sf::Vector2f playerPos, sf::Vector2f playerSize, sf::Vector2f bias) const;

    static void initializeMap(std::vector<std::vector<int>>& map, unsigned int seed, double INITIAL_PROB, int WIDTH, int HEIGHT);
    static void stepAutomaton(std::vector<std::vector<int>>& map, int WIDTH, int HEIGHT);

    void setLayer(int x, int y, int layer, int value);
    int getLayer(int x, int y, int layer) const;

    void draw(sf::RenderWindow &window, sf::Vector2f playerPos, sf::Vector2f view, int chunkSize);
    void loadChunksAroundPlayer(sf::Vector2f playerPos, int chunkSize, unsigned int seed);
    static std::vector<std::vector<int>> generateChunk(int chunkX, int chunkY, unsigned int seed, int chunkSize);
    void unloadDistantChunks(sf::Vector2f playerPos, int chunkSize);

    static void generateRivers(std::vector<std::vector<int>>& chunk, int chunkSize);
    static double generatePerlinNoise(double x, double y, double scale, int octaves, double persistence);
    void draw(sf::Vector2f playerPos, sf::RenderWindow &window);

private:
    void funkLoadChunksThread();
    void startChunkLoadingThread();
    void stopChunkLoadingThread();

    nlohmann::json objJson;
    std::fstream fileInput;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f PlayerPos;

    int imageHeight{};
    int imageWidth{};
    int distanceView;

    int LayerOdj[layerSizeMaxX][layerSizeMaxY]{};
    int LayerGround[layerSizeMaxX][layerSizeMaxY]{};

    std::map<std::pair<int, int>, std::vector<std::vector<int>>> chunks;
    std::vector<chunk> chunkVector;

    std::thread chunkLoadThread;
    std::mutex chunkMutex;
    bool ChunksThreadOnOff{false}; // Set to false initially
};
