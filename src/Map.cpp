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

double Map::fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double Map::lerp(double t, double a, double b) {
    return a + t * (b - a);
}

double Map::grad(int hash, double x, double y) {
    int h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : h == 12 || h == 14 ? x : 0;
    return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

double Map::perlin(double x, double y, int *permutation) {
    int xi = (int)x & 255;
    int yi = (int)y & 255;

    double xf = x - (int)x;
    double yf = y - (int)y;

    double u = fade(xf);
    double v = fade(yf);

    int aa = permutation[permutation[xi] + yi];
    int ab = permutation[permutation[xi] + yi + 1];
    int ba = permutation[permutation[xi + 1] + yi];
    int bb = permutation[permutation[xi + 1] + yi + 1];

    double x1 = lerp(u, grad(aa, xf, yf), grad(ba, xf - 1, yf));
    double x2 = lerp(u, grad(ab, xf, yf - 1), grad(bb, xf - 1, yf - 1));

    return lerp(v, x1, x2);
}

void Map::generatePermutation(int *permutation) {
    for (int i = 0; i < 256; ++i)
        permutation[i] = i;

    std::random_shuffle(&permutation[0], &permutation[256]);

    for (int i = 0; i < 256; ++i)
        permutation[256 + i] = permutation[i];
}


Chunk Map::generateChunk(int chunkX, int chunkY, int* permutation) {
    Chunk chunk;
    // Генерация чанка с шумом Перлина
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            double worldX = chunkX * 16 + x;
            double worldY = chunkY * 16 + y;
            double height = perlin(worldX * 0.1, worldY * 0.1, permutation);

            sf::RectangleShape tile(sf::Vector2f(32, 32));  // Размер тайла 32x32 пикселя
            tile.setPosition(worldX * 32, worldY * 32);
            tile.setFillColor(sf::Color(0, 255 * height, 0));  // Пример окрашивания
            chunk.tiles.push_back(tile);
        }
    }
    return chunk;
}

void Map::updateChunks(sf::Vector2f playerPosition, int* permutation) {
    int chunkX = static_cast<int>(playerPosition.x / (16 * 32));
    int chunkY = static_cast<int>(playerPosition.y / (16 * 32));

    std::vector<std::pair<int, int>> neededChunks;

    // Загрузка чанков вокруг игрока
    for (int y = -1; y <= 1; ++y) {
        for (int x = -1; x <= 1; ++x) {
            int cx = chunkX + x;
            int cy = chunkY + y;
            if (loadedChunks.find({cx, cy}) == loadedChunks.end()) {
                loadedChunks[{cx, cy}] = generateChunk(cx, cy, permutation);
            }
            neededChunks.push_back({cx, cy});
        }
    }

    // Выгрузка чанков, которые больше не нужны
    for (auto it = loadedChunks.begin(); it != loadedChunks.end();) {
        if (std::find(neededChunks.begin(), neededChunks.end(), it->first) == neededChunks.end()) {
            it = loadedChunks.erase(it);
        } else {
            ++it;
        }
    }
}

void Map::drawChunks(sf::RenderWindow& window) {
    for (const auto& chunk : loadedChunks) {
        for (const auto& tile : chunk.second.tiles) {
            window.draw(tile);
        }
    }
}