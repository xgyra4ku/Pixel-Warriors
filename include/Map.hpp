#pragma once
#include <SFML/Graphics.hpp>
#include "globals.hpp"
#include <fstream>
#include <string>
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
    bool checkingRangeOfNumbers(int a, int b, int c);

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


