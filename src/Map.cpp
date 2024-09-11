#include "../include/Map.hpp"
#include <iostream>
#include <cmath>

Map::Map() : m_iDistanceView(3), chunkLoadThread(), ChunksThreadOnOff(false), fileWorldIsOpen(false), m_bNewWorld(false) {
    //startChunkLoadingThread();
}

Map::~Map() {
    //stopChunkLoadingThread();
    //save();

    chunks.clear();
    chunkBufferLoadIsFile.clear();
    chunkBuffer.clear();
    chunkVector.clear();
    fileWorldIsOpen = false;
}
void Map::deleting() {
    save();
    chunks.clear();
    chunkBufferLoadIsFile.clear();
    chunkBuffer.clear();
    chunkVector.clear();
    fileWorldIsOpen = false;
    std::cout << "INFO: deleting" << std::endl;
}

void Map::load() {
    try {
        std::cout << "INFO: Loading Map" << std::endl;
        fileInput.open("./Maps/map1/map1.json");
        fileInput >> objJson;
        fileInput.close();
        auto layers = objJson["layers"];
        for (int i = 0; i < g_LayerSizeMaxY; i++) {
            auto odjData = layers[0]["data"];
            auto groundData = layers[1]["data"];
            for (int j = 0; j < g_LayerSizeMaxX; j++) {
                LayerOdj[i][j] = odjData[i * g_LayerSizeMaxX + j];
                LayerGround[i][j] = groundData[i * g_LayerSizeMaxX + j];
            }
        }
        std::cout << "INFO: Map loaded successfully" << std::endl;
    } catch (const nlohmann::json::exception& error) {
        std::cerr << "\033[31m" << "ERROR: Failed to load map " << error.what() << "\033[0m" << std::endl;
    }
}

double Map::generatePerlinNoise(const double x, const double y, const double scale, const int octaves, const double persistence, const unsigned int seed) {
    const PerlinNoise perlin(seed);
    return perlin.octave2D_01(x * scale, y * scale, octaves, persistence);
}

std::vector<std::vector<int>> Map::generateChunk(const int chunkX, const int chunkY, const unsigned int seed, const int chunkSize) {
    std::vector<std::vector<int>> chunk(chunkSize, std::vector<int>(chunkSize, 0));
    std::vector<std::vector<double>> noiseValues(chunkSize + 2, std::vector<double>(chunkSize + 2, 0.0));

    for (int y = -1; y <= chunkSize; ++y) {
        for (int x = -1; x <= chunkSize; ++x) {
            constexpr double persistence = 0.01;
            constexpr int octaves = 1;
            constexpr double scale = 0.001;
            const double noiseValue = generatePerlinNoise((x + chunkX * chunkSize), (y + chunkY * chunkSize), scale, octaves, persistence, seed);
            noiseValues[y + 1][x + 1] = noiseValue;
        }
    }
    for (int y = 0; y < chunkSize; ++y) {
        for (int x = 0; x < chunkSize; ++x) {
            if (const double noiseValue = noiseValues[y + 1][x + 1]; noiseValue < 0) {
                chunk[y][x] = 91;
            } else {
                chunk[y][x] = ((0 + rand() % 2) == 1) ? 143 : 106;
            }
        }
    }
    chunkAdaptation(noiseValues, chunk, chunkSize);

    return chunk;
}

void Map::generateRivers(std::vector<std::vector<int>>& chunk, const int chunkSize) {
    int riverStartX = rand() % chunkSize;
    int riverY = 0;

    while (riverY < chunkSize) {
        if (chunk[riverY][riverStartX] != 91) {
            chunk[riverY][riverStartX] = 91;
        }
        riverY++;
        const int direction = rand() % 3 - 1;
        riverStartX = std::max(0, std::min(chunkSize - 1, riverStartX + direction));
    }
}


void Map::chunkAdaptation(const std::vector<std::vector<double>>& noiseValues, std::vector<std::vector<int>>& chunk, const int chunkSize) {
    for (int y = 0; y < chunkSize; ++y) {
        for (int x = 0; x < chunkSize; ++x) {
            if (chunk[y][x] != 91) {
                //верх низ право лево
                if (noiseValues[y][x + 1] < 0 && noiseValues[y + 1][x] < 0) {
                    chunk[y][x] = 127;  // Верхний левый
                    continue;
                }
                if (noiseValues[y][x + 1] < 0 && noiseValues[y + 1][x + 2] < 0) {
                    chunk[y][x] = 114;  // Верхний правый
                    continue;
                }
                if (noiseValues[y + 2][x + 1] < 0 && noiseValues[y + 1][x + 2] < 0) {
                    chunk[y][x] = 144;  // Нижний правый
                    continue;
                }
                if (noiseValues[y + 2][x + 1] < 0 && noiseValues[y + 1][x] < 0) {
                    chunk[y][x] = 157;  // Нижний левый
                    continue;
                }

                // двойное соприкосновкение
                if (noiseValues[y + 1][x] < 0 && noiseValues[y + 1][x + 1] >= 0) {
                    chunk[y][x] = 142;  // Соприкосновение с водой слева
                    continue;
                }
                if (noiseValues[y + 1][x + 2] < 0 && noiseValues[y + 1][x + 1] >= 0) {
                    chunk[y][x] = 129;  // Соприкосновение с водой справа
                    continue;
                }
                if (noiseValues[y][x + 1] < 0 && noiseValues[y + 1][x + 1] >= 0) {
                    chunk[y][x] = 128;  // Соприкосновение с водой сверху
                    continue;
                }
                if (noiseValues[y + 2][x + 1] < 0 && noiseValues[y + 1][x + 1] >= 0) {
                    chunk[y][x] = 158;  // Соприкосновение с водой снизу
                    continue;
                }

                // Проверка диагонали: вода в верхнем левом углу
                if (noiseValues[y][x] < 0) {
                    chunk[y][x] = 153;  // Вода в верхнем левом углу
                    continue;
                }

                // Проверка диагонали: вода в верхнем правом углу
                if (noiseValues[y][x + 2] < 0) {
                    chunk[y][x] = 151;  // Вода в верхнем правом углу
                    continue;
                }

                // Проверка диагонали: вода в нижнем левом углу
                if (noiseValues[y + 2][x] < 0) {
                    chunk[y][x] = 123;  // Вода в нижнем левом углу
                    continue;
                }

                // Проверка диагонали: вода в нижнем правом углу
                if (noiseValues[y + 2][x + 2] < 0) {
                    chunk[y][x] = 121;  // Вода в нижнем правом углу
                }
            }
        }
    }
}
void Map::loadingChunksFromFile(const std::string& nameFile) {
    std::cout << "INFO: Open file " << nameFile << std::endl;
    fileInput.open("worlds/" + nameFile);
    if (!fileInput.is_open()) {
        std::cerr << "ERROR: opening file: " << nameFile << std::endl;
        return;
    }
    fileInput >> jsonLoad;
    fileInput.close();
    m_uiSeed = jsonLoad["seed"];

}
sf::Vector2f Map::getPosPlayer() {
    return {jsonLoad["player"]["pos"]["x"], jsonLoad["player"]["pos"]["y"]};
}

bool Map::checkingDownloadedChunks(const std::string& requiredChunk, std::vector<std::vector<int>>& chunkData) {
    if (jsonLoad.contains("chunks")) {
        if (jsonLoad["chunks"].contains(requiredChunk)) {
            if (const auto& chunk = jsonLoad["chunks"][requiredChunk]; chunk.contains("data")) {
                chunkData.clear();
                for (const auto& row : chunk["data"]) {
                    std::vector<int> rowData;
                    for (const auto& value : row) {
                        rowData.push_back(value);
                    }
                    chunkData.push_back(rowData);
                }
                return true;
            }
            std::cerr << "ERROR: The chunk '" << requiredChunk << "' does not contain the 'data' " << std::endl;
        }
        return false;
    }
    std::cerr << "ERROR: File is corrupted, there is no line 'chunks'" << std::endl;
    return false;
}

void Map::saveChunk(const int chunkX, const int chunkY, const std::vector<std::vector<int>>& data, const int chunkSize) {
    const std::string chunkName = std::to_string(chunkX) + "<>" + std::to_string(chunkY);
    chunkBuffer[chunkName] = data;
    if (size(chunkBuffer) > 300) save();
}

void Map::save() {
    if (!fileWorldIsOpen) {
        if (m_bNewWorld) {
            fileWorld.open("worlds/"+strNameFileWorld, std::ofstream::app);
            if(fileWorld.is_open()) {
                std::cout << "INFO: New file " << strNameFileWorld << " is open for writing" << std::endl;
                jsonSave = json::object();
                jsonSave["seed"] = m_uiSeed;
                jsonSave["chunks"] = json::object();
                m_bNewWorld = false;
                fileWorld.close();
            } else {
                std::cerr << "ERROR: Cannot open file " << strNameFileWorld << " for writing" <<  std::endl;
            }
        }
        fileWorld.open("worlds/"+strNameFileWorld);
        if (fileWorld.is_open()) {
            if (fileWorld.peek() == std::ifstream::traits_type::eof()) {
                std::cout << "INFO: The file " << strNameFileWorld << " is open for writing" << std::endl;
                fileWorld >> jsonSave;
            }
            fileWorld.close();
            fileWorldIsOpen = true;
        } else {
            std::cerr << "ERROR: Cannot open file " << strNameFileWorld << " for writing" <<  std::endl;
            return;
        }
    }

    std::cout << "INFO: Start save" << std::endl;

    for (const auto& [chunkName, data] : chunkBuffer) {
        json chunkData = json::array();
        for (const auto& row : data) {
            chunkData.push_back(row);
        }
        jsonSave["chunks"][chunkName]["data"] = chunkData;
    }

    jsonSave["player"]["pos"]["x"] = PlayerPos.x;
    jsonSave["player"]["pos"]["y"] = PlayerPos.y;

    fileWorld.open("worlds/" + strNameFileWorld, std::ofstream::out | std::ofstream::trunc);
    if (fileWorld.is_open()) {
        fileWorld << jsonSave.dump(4);
        fileWorld.close();
        std::cout << "INFO: Saved" << std::endl;
    } else {
        std::cerr << "ERROR: Cannot open file" << strNameFileWorld << " to save changes" << std::endl;
    }

    chunkBuffer.clear();
    loadingChunksFromFile(strNameFileWorld);
}

void Map::draw(sf::RenderWindow &window, const std::vector<std::vector<int>>& Layer, const sf::Vector2f playerPos, const sf::Vector2f viev) const {
    sf::VertexArray vertices(sf::Quads);
    const unsigned int textureSize = texture.getSize().x;

    int xMax = static_cast<int>((playerPos.x + viev.x) / g_dTileSize);
    int xMin = static_cast<int>((playerPos.x - viev.x) / g_dTileSize);
    int yMax = static_cast<int>((playerPos.y + viev.y) / g_dTileSize);
    int yMin = static_cast<int>((playerPos.y - viev.y) / g_dTileSize);

    xMax = std::min(xMax, g_LayerSizeMaxX);
    xMin = std::max(xMin, 0);
    yMax = std::min(yMax, g_LayerSizeMaxY);
    yMin = std::max(yMin, 0);

    for (int i = yMin; i < yMax; i++) {
        for (int j = xMin; j < xMax; j++) {
            if (Layer[i][j] != 0) {
                const int tileIndex = Layer[i][j] - 1;
                const int tilesPerRow = static_cast<int>(textureSize) / g_dTileSize;
                const auto tileX = static_cast<float>((tileIndex % tilesPerRow) * g_dTileSize);
                const auto tileY = static_cast<float>((tileIndex)) / static_cast<float>(tilesPerRow) * g_dTileSize;

                const float X = static_cast<float>(j) * g_dTileSize - g_fOffsetX;
                const float Y = static_cast<float>(i) * g_dTileSize - g_fOffsetY;

                vertices.append(sf::Vertex(sf::Vector2f(X, Y), sf::Vector2f(tileX, tileY)));
                vertices.append(sf::Vertex(sf::Vector2f(X + g_dTileSize, Y), sf::Vector2f(tileX + g_dTileSize, tileY)));
                vertices.append(sf::Vertex(sf::Vector2f(X + g_dTileSize, Y + g_dTileSize), sf::Vector2f(tileX + g_dTileSize, tileY + g_dTileSize)));
                vertices.append(sf::Vertex(sf::Vector2f(X, Y + g_dTileSize), sf::Vector2f(tileX, tileY + g_dTileSize)));
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

void Map::setLayer(const int x, const int y, const int layer, const int value) {
    if (layer == 0) {
        LayerGround[y][x] = value;
    } else if (layer == 1) {
        LayerOdj[y][x] = value;
    }
}

void Map::init(const int iDistanceView, const std::string& strNameFileMap, const sf::RenderWindow& window) {
    this->m_iDistanceView = iDistanceView;
    this->strNameFileWorld = strNameFileMap;
    loadingChunksFromFile(strNameFileMap);
    try {
        std::cout << "INFO: Loading tile set" << std::endl;

        fileInput.open("Maps/map2/3.json");
        fileInput >> objJson;
        fileInput.close();

        imageHeight = static_cast<int>(objJson["imageheight"]);
        imageWidth = static_cast<int>(objJson["imagewidth"]);

        if (!texture.loadFromFile("Maps/map2/3.png")) {
            std::cerr << "\033[31m" << "ERROR: Failed loading texture from file" << "\033[0m" << std::endl;
            return;
        }

        sprite.setTexture(texture);
        std::cout << "INFO: Tile set loaded successfully" << std::endl;
    }
    catch (const nlohmann::json::exception& error) {
        std::cerr << "\033[31m" << "ERROR: Failed to load tileset " << error.what() << "\033[30m" << std::endl;
    }
}
void Map::init(const int iDistanceView, const std::string& strNameFileMap, const sf::RenderWindow& window, const unsigned int seed) {
    this->m_uiSeed = seed;
    this->m_iDistanceView = iDistanceView;
    this->strNameFileWorld = strNameFileMap + ".json";
    m_bNewWorld = true;
    loadingChunksFromFile(strNameFileMap);
    try {
        std::cout << "INFO: Loading tile set" << std::endl;

        fileInput.open("Maps/map2/3.json");
        fileInput >> objJson;
        fileInput.close();

        imageHeight = static_cast<int>(objJson["imageheight"]);
        imageWidth = static_cast<int>(objJson["imagewidth"]);

        if (!texture.loadFromFile("Maps/map2/3.png")) {
            std::cerr << "\033[31m" << "ERROR: Failed loading texture from file" << "\033[0m" << std::endl;
            return;
        }

        sprite.setTexture(texture);
        std::cout << "INFO: Tile set loaded successfully" << std::endl;
    }
    catch (const nlohmann::json::exception& error) {
        std::cerr << "\033[31m" << "ERROR: Failed to load tileset " << error.what() << "\033[30m" << std::endl;
    }
}

int Map::collision(const sf::Vector2f playerPos, const sf::Vector2f playerSize, const sf::Vector2f bias) const {
    const sf::FloatRect playerBox((playerPos.x + bias.x) - g_fOffsetX, (playerPos.y + bias.y) - g_fOffsetY, playerSize.x, playerSize.y);
    for (int i = 0; i < g_LayerSizeMaxY; i++) {
        for (int j = 0; j < g_LayerSizeMaxX; j++) {
            if (LayerOdj[i][j] == 159) {
                if (const sf::FloatRect tileBox(static_cast<float>(j) * g_dTileSize - g_fOffsetX, static_cast<float>(i) * g_dTileSize - g_fOffsetY, g_dTileSize, g_dTileSize); playerBox.intersects(tileBox)) {
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

void Map::initializeMap(std::vector<std::vector<int>>& map, const unsigned int seed, const double INITIAL_PROB, const int WIDTH, const int HEIGHT) {
    std::srand(seed);
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            map[y][x] = (std::rand() / static_cast<double>(RAND_MAX)) < INITIAL_PROB ? 1 : 0;
        }
    }
}

void Map::stepAutomaton(std::vector<std::vector<int>>& map, const int WIDTH, const int HEIGHT) {
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

void Map::loadChunksAroundPlayer(const sf::Vector2f playerPos, const int chunkSize) {
    const int playerChunkX = static_cast<int>(playerPos.x) / (chunkSize * g_dTileSize);
    const int playerChunkY = static_cast<int>(playerPos.y) / (chunkSize * g_dTileSize);

    for (int y = -m_iDistanceView; y <= m_iDistanceView; ++y) {
        for (int x = -m_iDistanceView; x <= m_iDistanceView; ++x) {
            int chunkX = playerChunkX + x;
            int chunkY = playerChunkY + y;

            if (auto chunkKey = std::make_pair(chunkX, chunkY); chunks.find(chunkKey) == chunks.end()) {
                const std::string name = std::to_string(chunkX) + "<>" + std::to_string(chunkY);
                //std::cout << name << std::endl;
                if (std::vector<std::vector<int>> temp; checkingDownloadedChunks(name, temp)) {
                    chunks[chunkKey] = temp;
                } else {
                    chunks[chunkKey] = generateChunk(chunkX, chunkY, m_uiSeed, chunkSize);
                }
                saveChunk(chunkX, chunkY, chunks[chunkKey], chunkSize);
            }
        }
    }
}

void Map::unloadDistantChunks(const sf::Vector2f playerPos, const int chunkSize) {

    const int playerChunkX = static_cast<int>(playerPos.x) / (chunkSize * g_dTileSize);
    const int playerChunkY = static_cast<int>(playerPos.y) / (chunkSize * g_dTileSize);

    for (auto it = chunks.begin(); it != chunks.end();) {
        const int chunkX = it->first.first;

        if (const int chunkY = it->first.second; abs(chunkX - playerChunkX) > m_iDistanceView || abs(chunkY - playerChunkY) > m_iDistanceView) {
            auto chunkKey = std::make_pair(chunkX, chunkY);
            saveChunk(chunkX, chunkY, chunks[chunkKey], chunkSize);
            it = chunks.erase(it);
        } else {
            ++it;
        }
    }
}

void Map::draw(sf::RenderWindow &window, const sf::Vector2f playerPos, sf::Vector2f view, const int chunkSize) {
    // Lock the mutex while modifying chunks
   // std::lock_guard<std::mutex> lock(chunkMutex);
    this->PlayerPos = playerPos;
    loadChunksAroundPlayer(playerPos, chunkSize);

    const int playerChunkX = static_cast<int>(playerPos.x) / (chunkSize * g_dTileSize);
    const int playerChunkY = static_cast<int>(playerPos.y) / (chunkSize * g_dTileSize);

    sf::VertexArray vertices(sf::Quads);

    // Округляем offsetX и offsetY
    const float roundedOffsetX = std::round(g_fOffsetX);
    const float roundedOffsetY = std::round(g_fOffsetY);

    for (int y = -m_iDistanceView; y <= m_iDistanceView; ++y) {
        for (int x = -m_iDistanceView; x <= m_iDistanceView; ++x) {
            int chunkX = playerChunkX + x;
            int chunkY = playerChunkY + y;

            if (auto chunkKey = std::make_pair(chunkX, chunkY); chunks.find(chunkKey) != chunks.end()) {
                auto& chunk = chunks[chunkKey];
                for (int i = 0; i < chunkSize; ++i) {
                    for (int j = 0; j < chunkSize; ++j) {
                        if (const int tileValue = chunk[i][j]; tileValue != 0) {
                            const float X = static_cast<float>(j + chunkX * chunkSize) * g_dTileSize;
                            const float Y = static_cast<float>(i + chunkY * chunkSize) * g_dTileSize;

                            const int tilesPerRow = static_cast<int>(texture.getSize().x) / g_dTileSize;
                            const float tileX = static_cast<float>((tileValue - 1) % tilesPerRow) * g_dTileSize;
                            const float tileY = std::round(static_cast<float>(tileValue - 1) / static_cast<float>(tilesPerRow)) * g_dTileSize;

                            vertices.append(sf::Vertex(sf::Vector2f(X - roundedOffsetX, Y - roundedOffsetY), sf::Vector2f(tileX, tileY)));
                            vertices.append(sf::Vertex(sf::Vector2f(X + g_dTileSize - roundedOffsetX, Y - roundedOffsetY), sf::Vector2f(tileX + g_dTileSize, tileY)));
                            vertices.append(sf::Vertex(sf::Vector2f(X + g_dTileSize - roundedOffsetX, Y + g_dTileSize - roundedOffsetY), sf::Vector2f(tileX + g_dTileSize, tileY + g_dTileSize)));
                            vertices.append(sf::Vertex(sf::Vector2f(X - roundedOffsetX, Y + g_dTileSize - roundedOffsetY), sf::Vector2f(tileX, tileY + g_dTileSize)));
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

void Map::funkLoadChunksThread() {
    while (ChunksThreadOnOff) {
        // Lock the mutex while modifying chunks
        //std::lock_guard<std::mutex> lock_guard(chunkMutex);

        //loadChunksAroundPlayer(PlayerPos, 16);
        //unloadDistantChunks(PlayerPos, 16);

        //std::this_thread::sleep_for(std::chrono::seconds(1)); // Adjust as needed
    }
}

void Map::startChunkLoadingThread() {
    ChunksThreadOnOff = true;
    chunkLoadThread = std::thread(&Map::funkLoadChunksThread, this);
}

void Map::stopChunkLoadingThread() {
    ChunksThreadOnOff = false;
    if (chunkLoadThread.joinable()) {
        chunkLoadThread.join();
    }
}

void Map::createWorld() {

}