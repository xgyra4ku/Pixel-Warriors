#include "../include/Map.hpp"


Map::Map() {

}

Map::~Map() {

}

void Map::load()
{
    try {
        std::cout << "INFO: Loading Map" << std::endl;	
		fileInput.open("./Maps/map1/map1.json");
		fileInput >> objJson;
		fileInput.close();
		auto layers = objJson["layers"];
		for (int i = 0; i < layerSizeMaxY; i++) {
			auto odjData = layers[0]["data"];
			auto groundData = layers[1]["data"];
			for (int j = 0; j < layerSizeMaxX; j++) {
				LayerOdj[i][j] = odjData[i * layerSizeMaxX + j];
				LayerGround[i][j] = groundData[i * layerSizeMaxX + j];
			}
		}
        std::cout << "INFO: Map loaded successfully" << std::endl;  
	}
	catch (const nlohmann::json::exception& error) {
	    std::cerr << "ERROR: Failed to load map " << error.what() << std::endl;
	} 
}

void Map::save() {    

}


void Map::draw(sf::RenderWindow &window) {
    for (int i = 0; i < layerSizeMaxY; i++) {
        for (int j = 0; j < layerSizeMaxX; j++) {
            if (LayerGround[i][j] != 0) {
                int tileIndex = LayerGround[i][j] - 1;
                int tilesPerRow = texture.getSize().x / tileSize;
                int tileX = (tileIndex % tilesPerRow) * tileSize;
                int tileY = (tileIndex / tilesPerRow) * tileSize;

                sprite.setTextureRect(sf::IntRect(tileX, tileY, tileSize, tileSize));
                sprite.setPosition(j * tileSize - offsetX, i * tileSize - offsetY);
                window.draw(sprite);
            }
        }
    }
}

int Map::getLayer(int x, int y, int layer) {
    if (layer == 0) {
        return LayerGround[y][x];
    } else if (layer == 1) { 
        return LayerOdj[y][x];
    }
    if (layer >= 0 && layer < 2 && y >= 0 && y < layerSizeMaxY && x >= 0 && x < layerSizeMaxX) {
        if (layer == 0) {
            return LayerGround[y][x];
        } else if (layer == 1) {
            return LayerOdj[y][x];
        }
    }
    return -1; 
}

void Map::setLayer(int x, int y, int layer, int value) {
    if (layer == 0) {
        LayerGround[y][x] = value;
    } else if (layer == 1) {
        LayerOdj[y][x] = value;
    }
}

void Map::init() {
    try {
        std::cout << "INFO: Loading tileset" << std::endl;	
        fileInput.open("../Maps/map1/forest_.json");
        fileInput >> objJson;
        fileInput.close();

        imageheight = int(objJson["imageheight"]);
        imagewidth = int(objJson["imagewidth"]);

        
        if (!texture.loadFromFile("../Maps/map1/forest_.png")) {
            std::cerr << "ERROR: Failed loading texture from file" << std::endl;
            return;
        }
    
        sprite.setTexture(texture);

        std::cout << "INFO: Tileset loaded successfully" << std::endl;
    }
    catch (const nlohmann::json::exception& error) {
        std::cerr << "ERROR: Failed to load tileset " << error.what() << std::endl;
    } 
}

int Map::collision(sf::Vector2f playerPos, sf::Vector2f playerSize, sf::Vector2f bias) {
    sf::FloatRect playerBox((playerPos.x + bias.x) - offsetX, (playerPos.y + bias.y) - offsetY, playerSize.x, playerSize.y);

    for (int i = 0; i < layerSizeMaxY; i++) {
        for (int j = 0; j < layerSizeMaxX; j++) {
            if (LayerOdj[i][j] != 0) {
                sf::FloatRect tileBox(j * tileSize - offsetX, i * tileSize - offsetY, tileSize, tileSize);

                if (playerBox.intersects(tileBox)) {
                    float overlapLeft = playerBox.left + playerBox.width - tileBox.left;
                    float overlapRight = tileBox.left + tileBox.width - playerBox.left;
                    float overlapTop = playerBox.top + playerBox.height - tileBox.top;
                    float overlapBottom = tileBox.top + tileBox.height - playerBox.top;

                    bool collisionFromLeft = overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom;
                    bool collisionFromRight = overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom;
                    bool collisionFromTop = overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight;
                    bool collisionFromBottom = overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight;

                    if (collisionFromLeft) return 1; 
                    if (collisionFromRight) return 2; 
                    if (collisionFromTop) return 4; 
                    if (collisionFromBottom) return 3; 
                }
            }
        }
    }
    return 0;
}


bool Map::checkingRangeOfNumbers(int a, int b, int c) { 
    if (a >= b && a <= c) {
        return true;
    } 
    return false; 
}
