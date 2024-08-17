#pragma once
#include <SFML/Graphics.hpp>
#include "globals.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "nlohmann/json.hpp"
struct Chunk {
    std::vector<sf::RectangleShape> tiles;  // Замените на нужный тип данных для хранения тайлов
};


class Map
{
public:
    Map();
    ~Map();

    void init();
    void load();
    void save();
    void draw(sf::RenderWindow &window);
    void draw(sf::RenderWindow &window, std::vector<std::vector<int>>& Layer, sf::Vector2f playerPos, sf::Vector2f viev);
    int collision(sf::Vector2f playerPos, sf::Vector2f playerSize, sf::Vector2f bias);


    void initializeMap(std::vector<std::vector<int>>& map, unsigned int seed, double INITIAL_PROB, int WIDTH, int HEIGHT);
    int countLandNeighbors(const std::vector<std::vector<int>>& map, int x, int y, int WIDTH, int HEIGHT);
    void stepAutomaton(std::vector<std::vector<int>>& map, int WIDTH, int HEIGHT);

    void setLayer(int x, int y, int layer, int value);

    int getLayer(int x, int y, int layer);


    double fade(double t);
    double lerp(double t, double a, double b);
    double perlin(double x, double y, int *permutation);
    double grad(int hash, double x, double y);

    void generatePermutation(int *permutation);
    Chunk generateChunk(int chunkX, int chunkY, int* permutation);
    void updateChunks(sf::Vector2f playerPosition, int* permutation);
    void drawChunks(sf::RenderWindow& window);
private:
    nlohmann::json objJson;

    std::fstream fileInput;

    sf::Sprite sprite;
    sf::Texture texture;

    int imageheight;
    int imagewidth;

    int LayerOdj[layerSizeMaxX][layerSizeMaxY];
    int LayerGround[layerSizeMaxX][layerSizeMaxY];
    std::map<std::pair<int, int>, Chunk> loadedChunks;
};