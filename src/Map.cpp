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
                float tileX = float((tileIndex % tilesPerRow) * tileSize);
                float tileY = float((tileIndex / tilesPerRow) * tileSize);

                sprite.setTextureRect(sf::IntRect(tileX, tileY, tileSize, tileSize));
                sprite.setPosition(float(j) * tileSize - offsetX, float(i) * tileSize - offsetY);
                window.draw(sprite);
            }
        }
    }
}
void Map::draw(sf::RenderWindow &window, std::vector<std::vector<int>>& Layer, sf::Vector2f playerPos, sf::Vector2f viev) {
    int textureSize = texture.getSize().x;
    sf::VertexArray vertices(sf::Quads);

    int xMax = int((playerPos.x + viev.x) / tileSize);
    int xMin = int((playerPos.x - viev.x) / tileSize);
    int yMax = int((playerPos.y + viev.y) / tileSize);
    int yMin = int((playerPos.y - viev.y) / tileSize);

    if (xMax > layerSizeMaxX)
        xMax = layerSizeMaxX;
    if (xMin < 0)
        xMin = 0;
    if (yMax > layerSizeMaxY)
        yMax = layerSizeMaxY;
    if (yMin < 0)
        yMin = 0;

    for (int i = yMin; i < yMax; i++) {
        for (int j = xMin; j < xMax; j++) {
            if (Layer[i][j] != 0) {
                float Y = float(i) * tileSize;
                float X = float(j) * tileSize;

                int tileIndex = Layer[i][j] - 1;
                int tilesPerRow = textureSize / tileSize;
                float tileX = float((tileIndex % tilesPerRow) * tileSize);
                float tileY = float((tileIndex / tilesPerRow) * tileSize);

                // Создание четырех вершин (квадрата) для каждого тайла
                sf::Vertex vertex1, vertex2, vertex3, vertex4;

                vertex1.position = sf::Vector2f(X - offsetX, Y - offsetY);
                vertex2.position = sf::Vector2f(X + tileSize - offsetX, Y - offsetY);
                vertex3.position = sf::Vector2f(X + tileSize - offsetX, Y + tileSize - offsetY);
                vertex4.position = sf::Vector2f(X - offsetX, Y + tileSize - offsetY);

                vertex1.texCoords = sf::Vector2f(tileX, tileY);
                vertex2.texCoords = sf::Vector2f(tileX + tileSize, tileY);
                vertex3.texCoords = sf::Vector2f(tileX + tileSize, tileY + tileSize);
                vertex4.texCoords = sf::Vector2f(tileX, tileY + tileSize);

                vertices.append(vertex1);
                vertices.append(vertex2);
                vertices.append(vertex3);
                vertices.append(vertex4);
            }

        }
    }

    // Установка текстуры
    sf::RenderStates states;
    states.texture = &texture;

    // Отрисовка всех вершин за один вызов
    window.draw(vertices, states);
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
            if (LayerOdj[i][j] == 159) {
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

void Map::initializeMap(std::vector<std::vector<int>>& map, unsigned int seed, double INITIAL_PROB, int WIDTH, int HEIGHT) {
    std::srand(seed); // Инициализация генератора случайных чисел
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            // Заполняем карту случайными значениями на основе начальной вероятности суши
            map[y][x] = (std::rand() / (double)RAND_MAX) < INITIAL_PROB ? 1 : 0;
        }
    }
}

int Map::countLandNeighbors(const std::vector<std::vector<int>>& map, int x, int y, int WIDTH, int HEIGHT) {
    int count = 0; // Счетчик соседей-суши
    for (int dy = -1; dy <= 1; ++dy) { // Перебор соседей по вертикали
        for (int dx = -1; dx <= 1; ++dx) { // Перебор соседей по горизонтали
            if (dy == 0 && dx == 0) continue; // Пропуск самой клетки
            int nx = x + dx; // Координата x соседа
            int ny = y + dy; // Координата y соседа
            // Проверка, что сосед находится внутри карты
            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                count += map[ny][nx]; // Увеличение счетчика, если сосед - суша
            }
        }
    }
    return count; // Возвращаем количество соседей-суши
}

void Map::stepAutomaton(std::vector<std::vector<int>>& map, int WIDTH, int HEIGHT) {
    std::vector<std::vector<int>> newMap = map; // Создаем копию карты для обновлений

    for (int y = 0; y < HEIGHT; ++y) { // Перебор всех клеток по вертикали
        for (int x = 0; x < WIDTH; ++x) { // Перебор всех клеток по горизонтали
            int landNeighbors = countLandNeighbors(map, x, y, WIDTH, HEIGHT); // Подсчет соседей-суши
            if (map[y][x] == 1) { // Если текущая клетка - суша
                if (landNeighbors < 4) { // Меньше 4 соседей-суши
                    newMap[y][x] = 0; // Клетка становится водой
                }
            } else { // Если текущая клетка - вода
                if (landNeighbors > 4) { // Больше 4 соседей-суши
                    newMap[y][x] = 1; // Клетка становится сушей
                }
            }
        }
    }

    map = newMap; // Обновляем карту
}

// // Основная функция программы
// int main() {
//     std::vector<std::vector<int>> map(HEIGHT, std::vector<int>(WIDTH, 0)); // Инициализация карты
//     unsigned int seed = 12345; // Задайте ваше значение seed здесь

//     initializeMap(map, seed); // Инициализация карты случайными значениями

//     int generations = 10; // Количество поколений клеточного автомата

//     // Запуск клеточного автомата на заданное количество поколений
//     for (int gen = 0; gen < generations; ++gen) {
//         stepAutomaton(map); // Выполнение одного шага клеточного автомата
//     }
// }
