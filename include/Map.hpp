#pragma once
#include <SFML/Graphics.hpp>
#include "globals.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "nlohmann/json.hpp"


class Map
{
    public:
    Map();
    ~Map();

    void init();
    void load();
    void save();
    void draw(sf::RenderWindow &window);
    int collision(sf::Vector2f playerPos, sf::Vector2f playerSize, sf::Vector2f bias);


    void stepAutomaton(std::vector<std::vector<int>>& map, const int WIDTH, const int HEIGHT);
    int countLandNeighbors(const std::vector<std::vector<int>>& map, int x, int y, const int WIDTH, const int HEIGHT);
    void initializeMap(std::vector<std::vector<int>>& map, unsigned int seed, const double INITIAL_PROB, const int WIDTH, const int HEIGHT);

    void setLayer(int x, int y, int layer, int value);

    int getLayer(int x, int y, int layer);
private:
    nlohmann::json objJson;
    
	std::fstream fileInput;

    sf::Sprite sprite;
    sf::Texture texture;

    int imageheight;
    int imagewidth;
    
    int LayerOdj[layerSizeMaxX][layerSizeMaxY];
	int LayerGround[layerSizeMaxX][layerSizeMaxY];


};


