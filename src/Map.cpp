#include "../include/Map.hpp"


Map::Map() : distanceView(3) {}

Map::~Map() = default;

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

double Map::generatePerlinNoise(const double x,const double y,const double scale,const int octaves,const double persistence) {
    const PerlinNoise perlin(555555);
    return perlin.octave2D_01(x * scale, y * scale, octaves, persistence);
}

std::vector<std::vector<int>> Map::generateChunk(const int chunkX, const int chunkY, unsigned int seed, const int chunkSize) {
    std::vector<std::vector<int>> chunk(chunkSize, std::vector<int>(chunkSize, 0));

    for (int y = 0; y < chunkSize; ++y) {
        for (int x = 0; x < chunkSize; ++x) {
            constexpr double scale = 0.001;
            constexpr int octaves = 1;
            constexpr double persistence = 0.01;
            const double noiseValue = generatePerlinNoise(x + chunkX * chunkSize, y + chunkY * chunkSize, scale, octaves, persistence);
            chunk[y][x] = (noiseValue < 0) ? 159 : 47; // Вода или земля
        }
    }
    return chunk;
}

void Map::generateRivers(std::vector<std::vector<int>>& chunk,const int chunkSize) {
    int riverStartX = rand() % chunkSize;
    int riverY = 0;

    while (riverY < chunkSize) {
        if (chunk[riverY][riverStartX] != 159) {
            chunk[riverY][riverStartX] = 159;
        }
        riverY++;
        const int direction = rand() % 3 - 1;
        riverStartX = std::max(0, std::min(chunkSize - 1, riverStartX + direction));
    }
}

void Map::save() {
    // Реализация сохранения карты
}

void Map::draw(sf::RenderWindow &window, const std::vector<std::vector<int>>& Layer, const sf::Vector2f playerPos, const sf::Vector2f viev) const {
    sf::VertexArray vertices(sf::Quads);
    const unsigned int textureSize = texture.getSize().x;

    int xMax = static_cast<int>((playerPos.x + viev.x) / tileSize);
    int xMin = static_cast<int>((playerPos.x - viev.x) / tileSize);
    int yMax = static_cast<int>((playerPos.y + viev.y) / tileSize);
    int yMin = static_cast<int>((playerPos.y - viev.y) / tileSize);

    xMax = std::min(xMax, layerSizeMaxX);
    xMin = std::max(xMin, 0);
    yMax = std::min(yMax, layerSizeMaxY);
    yMin = std::max(yMin, 0);

    for (int i = yMin; i < yMax; i++) {
        for (int j = xMin; j < xMax; j++) {
            if (Layer[i][j] != 0) {
                const int tileIndex = Layer[i][j] - 1;
                const int tilesPerRow = static_cast<int>(textureSize) / tileSize;
                const auto tileX = static_cast<float>((tileIndex % tilesPerRow) * tileSize);
                const auto tileY = static_cast<float>((tileIndex)) / static_cast<float>(tilesPerRow) * tileSize;

                const float X = static_cast<float>(j) * tileSize - offsetX;
                const float Y = static_cast<float>(i) * tileSize - offsetY;

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

int Map::getLayer(const int x, const int y, const int layer) const {
    if (layer == 0) return LayerGround[y][x];
    if (layer == 1) return LayerOdj[y][x];
    return -1;
}

void Map::setLayer(const int x,const int y, const int layer,const int value) {
    if (layer == 0) {
        LayerGround[y][x] = value;
    } else if (layer == 1) {
        LayerOdj[y][x] = value;
    }
}

void Map::init(const int distanceView) {
    this->distanceView = distanceView;
    try {
        std::cout << "INFO: Loading tileset" << std::endl;

        fileInput.open("../Maps/map1/forest_.json");
        fileInput >> objJson;
        fileInput.close();

        imageHeight = static_cast<int>(objJson["imageheight"]);
        imageWidth = static_cast<int>(objJson["imagewidth"]);

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

int Map::collision(const sf::Vector2f playerPos,const sf::Vector2f playerSize,const sf::Vector2f bias) const {
    const sf::FloatRect playerBox((playerPos.x + bias.x) - offsetX, (playerPos.y + bias.y) - offsetY, playerSize.x, playerSize.y);

    for (int i = 0; i < layerSizeMaxY; i++) {
        for (int j = 0; j < layerSizeMaxX; j++) {
            if (LayerOdj[i][j] == 159) {
                if (const sf::FloatRect tileBox(static_cast<float>(j) * tileSize - offsetX, static_cast<float>(i) * tileSize - offsetY, tileSize, tileSize); playerBox.intersects(tileBox)) {
                    const float overlapLeft = playerBox.left + playerBox.width - tileBox.left;
                    const float overlapRight = tileBox.left + tileBox.width - playerBox.left;
                    const float overlapTop = playerBox.top + playerBox.height - tileBox.top;
                    const float overlapBottom = tileBox.top + tileBox.height - playerBox.top;

                    const bool collisionFromLeft = overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom;
                    const bool collisionFromRight = overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom;
                    const bool collisionFromTop = overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight;
                    const bool collisionFromBottom = overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight;

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

void Map::initializeMap(std::vector<std::vector<int>>& map,const unsigned int seed,const double INITIAL_PROB,const int WIDTH,const int HEIGHT) {
    std::srand(seed);
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            map[y][x] = (std::rand() / static_cast<double>(RAND_MAX)) < INITIAL_PROB ? 1 : 0;
        }
    }
}

void Map::stepAutomaton(std::vector<std::vector<int>>& map, const int WIDTH,const int HEIGHT) {
    std::vector<std::vector<int>> newMap = map;

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int count = 0;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dy == 0 && dx == 0) continue;
                    const int nx = x + dx;
                    if (const int ny = y + dy; nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
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

void Map::loadChunksAroundPlayer(const sf::Vector2f playerPos, const int chunkSize, const unsigned int seed) {
    const int playerChunkX = static_cast<int>(playerPos.x) / (chunkSize * tileSize);
    const int playerChunkY = static_cast<int>(playerPos.y) / (chunkSize * tileSize);

    for (int y = -distanceView; y <= distanceView; ++y) {
        for (int x = -distanceView; x <= distanceView; ++x) {
            int chunkX = playerChunkX + x;
            int chunkY = playerChunkY + y;

            if (auto chunkKey = std::make_pair(chunkX, chunkY); chunks.find(chunkKey) == chunks.end()) {
                chunks[chunkKey] = generateChunk(chunkX, chunkY, seed, chunkSize);
            }
        }
    }
}

void Map::unloadDistantChunks(const sf::Vector2f playerPos, const int chunkSize) {
    const int playerChunkX = static_cast<int>(playerPos.x) / (chunkSize * tileSize);
    const int playerChunkY = static_cast<int>(playerPos.y) / (chunkSize * tileSize);

    for (auto it = chunks.begin(); it != chunks.end();) {
        const int chunkX = it->first.first;

        if (const int chunkY = it->first.second; abs(chunkX - playerChunkX) > distanceView || abs(chunkY - playerChunkY) > distanceView) {
            it = chunks.erase(it);
        } else {
            ++it;
        }
    }
}
void Map::draw(sf::RenderWindow &window, const sf::Vector2f playerPos, sf::Vector2f view, const int chunkSize) {
    loadChunksAroundPlayer(playerPos, chunkSize, 12345);

    const int playerChunkX = static_cast<int>(playerPos.x) / (chunkSize * tileSize);
    const int playerChunkY = static_cast<int>(playerPos.y) / (chunkSize * tileSize);

    sf::VertexArray vertices(sf::Quads);

    // Округляем offsetX и offsetY
    const float roundedOffsetX = std::round(offsetX);
    const float roundedOffsetY = std::round(offsetY);

    for (int y = -distanceView; y <= distanceView; ++y) {
        for (int x = -distanceView; x <= distanceView; ++x) {
            int chunkX = playerChunkX + x;
            int chunkY = playerChunkY + y;

            if (auto chunkKey = std::make_pair(chunkX, chunkY); chunks.find(chunkKey) != chunks.end()) {
                auto& chunk = chunks[chunkKey];
                for (int i = 0; i < chunkSize; ++i) {
                    for (int j = 0; j < chunkSize; ++j) {
                        if (const int tileValue = chunk[i][j]; tileValue != 0) {
                            const float X = static_cast<float>(j + chunkX * chunkSize) * tileSize;
                            const float Y = static_cast<float>(i + chunkY * chunkSize) * tileSize;

                            const int tilesPerRow = static_cast<int>(texture.getSize().x) / tileSize;
                            const float tileX = static_cast<float>((tileValue - 1) % tilesPerRow) * tileSize;
                            const float tileY = std::round(static_cast<float>(tileValue - 1) / static_cast<float>(tilesPerRow)) * tileSize;

                            vertices.append(sf::Vertex(sf::Vector2f(X - roundedOffsetX, Y - roundedOffsetY), sf::Vector2f(tileX, tileY)));
                            vertices.append(sf::Vertex(sf::Vector2f(X + tileSize - roundedOffsetX, Y - roundedOffsetY), sf::Vector2f(tileX + tileSize, tileY)));
                            vertices.append(sf::Vertex(sf::Vector2f(X + tileSize - roundedOffsetX, Y + tileSize - roundedOffsetY), sf::Vector2f(tileX + tileSize, tileY + tileSize)));
                            vertices.append(sf::Vertex(sf::Vector2f(X - roundedOffsetX, Y + tileSize - roundedOffsetY), sf::Vector2f(tileX, tileY + tileSize)));
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
