#include "../include/Map.hpp"

Map::Map() : distanceView(3), stopThread(false) {
    ChunkGenerationThread = std::thread(&Map::, this);
}

Map::~Map() {
    stopThread = true;
    if (ChunkGenerationThread.joinable()) {
        ChunkGenerationThread.join();
    }
}

void Map::load() {
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
    } catch (const nlohmann::json::exception& error) {
        std::cerr << "ERROR: Failed to load map " << error.what() << std::endl;
    }
}

double Map::generatePerlinNoise(double x, double y, double scale, int octaves, double persistence) {
    PerlinNoise perlin(555555);
    return perlin.octave2D_01(x * scale, y * scale, octaves, persistence);
}

std::vector<std::vector<int>> Map::generateChunk(int chunkX, int chunkY, unsigned int seed, int chunkSize) {
    std::vector<std::vector<int>> chunk(chunkSize, std::vector<int>(chunkSize, 0));
    double scale = 0.005;
    int octaves = 1;
    double persistence = 0.005;

    for (int y = 0; y < chunkSize; ++y) {
        for (int x = 0; x < chunkSize; ++x) {
            double noiseValue = generatePerlinNoise(x + chunkX * chunkSize, y + chunkY * chunkSize, scale, octaves, persistence);
            if (noiseValue < 0) {
                chunk[y][x] = 159;
            } else if (noiseValue < 0.1) {
                chunk[y][x] = 100;
            } else {
                chunk[y][x] = 47;
            }
        }
    }
    return chunk;
}

void Map::generateRivers(std::vector<std::vector<int>>& chunk, int chunkSize) {
    int riverStartX = rand() % chunkSize;
    int riverY = 0;

    while (riverY < chunkSize) {
        if (chunk[riverY][riverStartX] != 159) {
            chunk[riverY][riverStartX] = 159;
        }
        riverY++;
        int direction = rand() % 3 - 1;
        riverStartX = std::max(0, std::min(chunkSize - 1, riverStartX + direction));
    }
}

void Map::save() {
    // Реализация сохранения карты
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

void Map::draw(sf::RenderWindow &window, std::vector<std::vector<int>>& Layer, sf::Vector2f playerPos, sf::Vector2f view) {
    sf::VertexArray vertices(sf::Quads);
    int textureSize = texture.getSize().x;

    int xMax = int((playerPos.x + view.x) / tileSize);
    int xMin = int((playerPos.x - view.x) / tileSize);
    int yMax = int((playerPos.y + view.y) / tileSize);
    int yMin = int((playerPos.y - view.y) / tileSize);

    xMax = std::min(xMax, layerSizeMaxX);
    xMin = std::max(xMin, 0);
    yMax = std::min(yMax, layerSizeMaxY);
    yMin = std::max(yMin, 0);

    for (int i = yMin; i < yMax; i++) {
        for (int j = xMin; j < xMax; j++) {
            if (Layer[i][j] != 0) {
                int tileIndex = Layer[i][j] - 1;
                int tilesPerRow = textureSize / tileSize;
                float tileX = float((tileIndex % tilesPerRow) * tileSize);
                float tileY = float((tileIndex / tilesPerRow) * tileSize);

                float X = float(j) * tileSize - offsetX;
                float Y = float(i) * tileSize - offsetY;

                vertices.append(sf::Vertex(sf::Vector2f(X, Y), sf::Vector2f(tileX, tileY)));
                vertices.append(sf::Vertex(sf::Vector2f(X + tileSize, Y), sf::Vector2f(tileX + tileSize, tileY)));
                vertices.append(sf::Vertex(sf::Vector2f(X + tileSize, Y + tileSize), sf::Vector2f(tileX + tileSize, tileY + tileSize)));
                vertices.append(sf::Vertex(sf::Vector2f(X, Y + tileSize), sf::Vector2f(tileX, tileY + tileSize)));
            }
        }
    }

    sf::RenderStates states;
    states.texture = &texture;
    window.draw(vertices, states);
}

int Map::getLayer(int x, int y, int layer) {
    if (layer == 0) return LayerGround[y][x];
    if (layer == 1) return LayerOdj[y][x];
    return -1;
}

void Map::setLayer(int x, int y, int layer, int value) {
    if (layer == 0) {
        LayerGround[y][x] = value;
    } else if (layer == 1) {
        LayerOdj[y][x] = value;
    }
}

void Map::init(int distanceView) {
    this->distanceView = distanceView;
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
    std::srand(seed);
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            map[y][x] = (std::rand() / (double)RAND_MAX) < INITIAL_PROB ? 1 : 0;
        }
    }
}

void Map::stepAutomaton(std::vector<std::vector<int>>& map, int WIDTH, int HEIGHT) {
    std::vector<std::vector<int>> newMap = map;

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int count = 0;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dy == 0 && dx == 0) continue;
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                        count += map[ny][nx];
                    }
                }
            }

            if (map[y][x] == 1) {
                if (count < 4) {
                    newMap[y][x] = 0;
                }
            } else {
                if (count > 4) {
                    newMap[y][x] = 1;
                }
            }
        }
    }

    map = newMap;
}

void Map::loadChunksAroundPlayer(sf::Vector2f playerPos, int chunkSize, unsigned int seed) {
    int playerChunkX = int(playerPos.x) / (chunkSize * tileSize);
    int playerChunkY = int(playerPos.y) / (chunkSize * tileSize);

    for (int y = -distanceView; y <= distanceView; ++y) {
        for (int x = -distanceView; x <= distanceView; ++x) {
            int chunkX = playerChunkX + x;
            int chunkY = playerChunkY + y;

            auto chunkKey = std::make_pair(chunkX, chunkY);
            if (chunks.find(chunkKey) == chunks.end()) {
                chunks[chunkKey] = generateChunk(chunkX, chunkY, seed, chunkSize);
            }
        }
    }
}

void Map::unloadDistantChunks(sf::Vector2f playerPos, int chunkSize) {
    int playerChunkX = int(playerPos.x) / (chunkSize * tileSize);
    int playerChunkY = int(playerPos.y) / (chunkSize * tileSize);

    for (auto it = chunks.begin(); it != chunks.end();) {
        int chunkX = it->first.first;
        int chunkY = it->first.second;

        if (abs(chunkX - playerChunkX) > distanceView || abs(chunkY - playerChunkY) > distanceView) {
            it = chunks.erase(it);
        } else {
            ++it;
        }
    }
}

void Map::draw(sf::RenderWindow &window, sf::Vector2f playerPos, sf::Vector2f view, int chunkSize) {
    loadChunksAroundPlayer(playerPos, chunkSize, 12345);

    int playerChunkX = int(playerPos.x) / (chunkSize * tileSize);
    int playerChunkY = int(playerPos.y) / (chunkSize * tileSize);

    sf::VertexArray vertices(sf::Quads);

    for (int y = -distanceView; y <= distanceView; ++y) {
        for (int x = -distanceView; x <= distanceView; ++x) {
            int chunkX = playerChunkX + x;
            int chunkY = playerChunkY + y;

            auto chunkKey = std::make_pair(chunkX, chunkY);
            if (chunks.find(chunkKey) != chunks.end()) {
                auto& chunk = chunks[chunkKey];
                for (int i = 0; i < chunkSize; ++i) {
                    for (int j = 0; j < chunkSize; ++j) {
                        int tileValue = chunk[i][j];
                        if (tileValue != 0) {
                            float X = float(j + chunkX * chunkSize) * tileSize;
                            float Y = float(i + chunkY * chunkSize) * tileSize;

                            int tilesPerRow = texture.getSize().x / tileSize;
                            float tileX = float((tileValue - 1) % tilesPerRow) * tileSize;
                            float tileY = float((tileValue - 1) / tilesPerRow) * tileSize;

                            vertices.append(sf::Vertex(sf::Vector2f(X - offsetX, Y - offsetY), sf::Vector2f(tileX, tileY)));
                            vertices.append(sf::Vertex(sf::Vector2f(X + tileSize - offsetX, Y - offsetY), sf::Vector2f(tileX + tileSize, tileY)));
                            vertices.append(sf::Vertex(sf::Vector2f(X + tileSize - offsetX, Y + tileSize - offsetY), sf::Vector2f(tileX + tileSize, tileY + tileSize)));
                            vertices.append(sf::Vertex(sf::Vector2f(X - offsetX, Y + tileSize - offsetY), sf::Vector2f(tileX, tileY + tileSize)));
                        }
                    }
                }
            }
        }
    }

    sf::RenderStates states;
    states.texture = &texture;
    window.draw(vertices, states);

    unloadDistantChunks(playerPos, chunkSize);
}
void Map::chunkLoader() {
    while (!stopThread) {
        std::pair<int, int> chunkKey;
        {
            std::lock_guard<std::mutex> lock(chunkMutex);
            if (!chunksToLoad.empty()) {
                chunkKey = chunksToLoad.front();
                chunksToLoad.pop();
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
        }

        int chunkSize = 16; // размер чанка
        unsigned int seed = 12345;
        auto chunk = generateChunk(chunkKey.first, chunkKey.second, seed, chunkSize);

        std::lock_guard<std::mutex> lock(chunkMutex);
        chunks[chunkKey] = std::move(chunk);
    }
}