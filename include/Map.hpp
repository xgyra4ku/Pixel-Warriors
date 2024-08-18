#pragma once
#include <SFML/Graphics.hpp>
#include "globals.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "nlohmann/json.hpp"
#include <map>
#include <unordered_map>
#include <utility>
#include <thread>
#include <mutex>
#include <queue>
#include "PerlinNoise.hpp"

class Map
{
public:
    Map();
    ~Map();

    void init(int distanceView);
    void load();
    void save();
    void draw(sf::RenderWindow &window);
    void draw(sf::RenderWindow &window, std::vector<std::vector<int>>& Layer, sf::Vector2f playerPos, sf::Vector2f viev);
    int collision(sf::Vector2f playerPos, sf::Vector2f playerSize, sf::Vector2f bias);


    void initializeMap(std::vector<std::vector<int>>& map, unsigned int seed, double INITIAL_PROB, int WIDTH, int HEIGHT);
    //int countLandNeighbors(const std::vector<std::vector<int>>& map, int x, int y, int WIDTH, int HEIGHT);
    void stepAutomaton(std::vector<std::vector<int>>& map, int WIDTH, int HEIGHT);

    void setLayer(int x, int y, int layer, int value);

    int getLayer(int x, int y, int layer);



    void draw(sf::RenderWindow &window, sf::Vector2f playerPos, sf::Vector2f view, int chunkSize);
    void loadChunksAroundPlayer(sf::Vector2f playerPos, int chunkSize, unsigned int seed);
    std::vector<std::vector<int>> generateChunk(int chunkX, int chunkY, unsigned int seed, int chunkSize);
    void unloadDistantChunks(sf::Vector2f playerPos, int chunkSize);

    void generateRivers(std::vector<std::vector<int>>& chunk, int chunkSize);
    double generatePerlinNoise(double x, double y, double scale, int octaves, double persistence);



private:
    nlohmann::json objJson;

    std::fstream fileInput;

    sf::Sprite sprite;
    sf::Texture texture;

    bool stopThread;
    std::thread ChunkGenerationThread;

    int imageheight;
    int imagewidth;
    int distanceView;

    int LayerOdj[layerSizeMaxX][layerSizeMaxY];
    int LayerGround[layerSizeMaxX][layerSizeMaxY];

    std::map<std::pair<int, int>, std::vector<std::vector<int>>> chunks;



};