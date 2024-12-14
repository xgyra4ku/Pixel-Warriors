#include "../include/cMap.hpp"
#include <iostream>
#include <cmath>
#include <random>
//
// Конструктор с установкой значениями переменых
//
cMap::cMap() : thread(&cMap::chunkLoadUnloadThread, this), m_iDistanceView(3), fileWorldIsOpen(false), m_bNewWorld(false), isThreadRunning(false), isThreadStarted(false) {}

//
// Дистуктор
//
cMap::~cMap() {
    //stopChunkLoadingThread();
    //save();

    chunks.clear();
    chunkBufferLoadIsFile.clear();
    chunkBuffer.clear();
    chunkVector.clear();
    fileWorldIsOpen = false;
    thread.terminate();
}

//
// Удаление старых данных
//
void cMap::deleting() {
    save();
    isThreadStarted = false;
    chunks.clear();
    chunkBufferLoadIsFile.clear();
    chunkBuffer.clear();
    chunkVector.clear();
    fileWorldIsOpen = false;
    oCmdInfo.info("Deleting and saving");
}

//
// генирация тайла
//
double cMap::generatePerlinNoise(const double x, const double y, const double scale, const int octaves, const double persistence, const unsigned int seed) {
    const PerlinNoise perlin(seed);
    return perlin.octave2D_01(x * scale, y * scale, octaves, persistence);
}

//
// Генирация чанков
//
ChunkStruct cMap::generateChunk(const int chunkX, const int chunkY, const unsigned int seed, const int chunkSize) {
    // Инициализация структур чанков и шумов
    std::vector<std::vector<int>> chunk(chunkSize, std::vector<int>(chunkSize, 0));
    std::vector<std::vector<int>> chunkObjects(chunkSize, std::vector<int>(chunkSize, 0));
    std::vector<std::vector<double>> noiseValuesObject((chunkSize / 2) + 2, std::vector<double>((chunkSize / 2) + 2, 0.0));
    std::vector<std::vector<double>> noiseValues(chunkSize + 2, std::vector<double>(chunkSize + 2, 0.0));

    // Генерация основного шума
    constexpr double persistence = 0.01;
    constexpr int octaves = 1;
    constexpr double scale = 0.001;

    for (int y = -1; y <= chunkSize; ++y) {
        for (int x = -1; x <= chunkSize; ++x) {
            double noiseValue = generatePerlinNoise(
                (x + chunkX * chunkSize),
                (y + chunkY * chunkSize),
                scale, octaves, persistence, seed
            );
            noiseValues[y + 1][x + 1] = noiseValue;
        }
    }

    // Генерация шума объектов
    for (int y = -1; y <= chunkSize / 2; ++y) {
        for (int x = -1; x <= chunkSize / 2; ++x) {
            double noiseValueObject = generatePerlinNoise(
                (x + chunkX * chunkSize),
                (y + chunkY * chunkSize),
                scale, octaves, persistence,
                seed + static_cast<unsigned int>(6.5 / seed)
            );
            noiseValuesObject[y + 1][x + 1] = noiseValueObject;
        }
    }

    // Заполнение данных чанка
    for (int y = 0; y < chunkSize; ++y) {
        for (int x = 0; x < chunkSize; ++x) {
            const double noiseValue = noiseValues[y + 1][x + 1];
            chunk[y][x] = (noiseValue < 0) ? 91 : ((rand() % 2 == 0) ? 143 : 106);
        }
    }

    // Заполнение объектов чанка
    for (int y = 0; y < chunkSize; ++y) {
        for (int x = 0; x < chunkSize; ++x) {
            const double noiseValueObject = noiseValues[y + 1][x + 1];
            chunkObjects[y][x] = (noiseValueObject < 0.1) ? 0 : 1;
        }
    }

    // Адаптация чанка
    chunkAdaptation(noiseValues, chunk, chunkSize);

    // Подготовка и возврат структуры
    ChunkStruct chunkStruct;
    chunkStruct.data = chunk;
    chunkStruct.dataObjects = chunkObjects;
    return chunkStruct;
}


//
// Генирация рек
//
void cMap::generateRivers(std::vector<std::vector<int>>& chunk, const int chunkSize) {
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


//
// Рефакторинко и адаптация чанка
//
void cMap::chunkAdaptation(const std::vector<std::vector<double>>& noiseValues, std::vector<std::vector<int>>& chunk, const int chunkSize) {
    for (int y = 0; y < chunkSize; ++y) {
        for (int x = 0; x < chunkSize; ++x) {
            if (chunk[y][x] == 91) {
                continue;
            }

            // Верхний левый угол
            if (noiseValues[y][x + 1] < 0 && noiseValues[y + 1][x] < 0) {
                chunk[y][x] = 127;
                continue;
            }
            // Верхний правый угол
            if (noiseValues[y][x + 1] < 0 && noiseValues[y + 1][x + 2] < 0) {
                chunk[y][x] = 114;
                continue;
            }
            // Нижний правый угол
            if (noiseValues[y + 2][x + 1] < 0 && noiseValues[y + 1][x + 2] < 0) {
                chunk[y][x] = 144;
                continue;
            }
            // Нижний левый угол
            if (noiseValues[y + 2][x + 1] < 0 && noiseValues[y + 1][x] < 0) {
                chunk[y][x] = 157;
                continue;
            }

            // Соприкосновение с водой
            if (noiseValues[y + 1][x] < 0 && noiseValues[y + 1][x + 1] >= 0) {
                chunk[y][x] = 142; // Слева
                continue;
            }
            if (noiseValues[y + 1][x + 2] < 0 && noiseValues[y + 1][x + 1] >= 0) {
                chunk[y][x] = 129; // Справа
                continue;
            }
            if (noiseValues[y][x + 1] < 0 && noiseValues[y + 1][x + 1] >= 0) {
                chunk[y][x] = 128; // Сверху
                continue;
            }
            if (noiseValues[y + 2][x + 1] < 0 && noiseValues[y + 1][x + 1] >= 0) {
                chunk[y][x] = 158; // Снизу
                continue;
            }

            // Диагонали
            if (noiseValues[y][x] < 0) {
                chunk[y][x] = 153; // Верхний левый
                continue;
            }
            if (noiseValues[y][x + 2] < 0) {
                chunk[y][x] = 151; // Верхний правый
                continue;
            }
            if (noiseValues[y + 2][x] < 0) {
                chunk[y][x] = 123; // Нижний левый
                continue;
            }
            if (noiseValues[y + 2][x + 2] < 0) {
                chunk[y][x] = 121; // Нижний правый
            }
        }
    }
}

//
// Выгрузка мира из чанка
//
void cMap::loadingChunksFromFile() {
    std::cout << "INFO: Open file " << strNameFileWorld << std::endl;
    fileInput.open("worlds/" + strNameFileWorld);
    if (!fileInput.is_open()) {
        oCmdInfo.error("Opening file: " + strNameFileWorld);
        return;
    }
    fileInput >> jsonLoad;
    fileInput.close();
    m_uiSeed = jsonLoad["seed"];

}

///
/// Получения позиции игрока из мира
///
sf::Vector2f cMap::getPosPlayer() {
    return {jsonLoad["player"]["pos"]["x"], jsonLoad["player"]["pos"]["y"]};
}

/**
 * Проверяет, загружены ли необходимые чанки из файла.
 *
 * @param requiredChunk Имя чанка, который необходимо проверить.
 * @param chunkData Вектор, в который будут записаны данные чанка.
 *
 * @return True, если чанк загружен успешно, false в противном случае.
 */
bool cMap::checkingDownloadedChunks(const std::string& requiredChunk, std::vector<std::vector<int>>& chunkData) {
    // Проверяем, содержит ли файл раздел "chunks"
    if (jsonLoad.contains("chunks")) {
        // Проверяем, содержит ли раздел "chunks" необходимый чанк
        if (jsonLoad["chunks"].contains(requiredChunk)) {
            // Получаем данные чанка
            if (const auto& chunk = jsonLoad["chunks"][requiredChunk]; chunk.contains("data")) {
                // Очищаем вектор для записи данных чанка
                chunkData.clear();
                // Перебираем строки данных чанка
                for (const auto& row : chunk["data"]) {
                    // Создаём временный вектор для строки данных
                    std::vector<int> rowData;
                    // Перебираем значения в строке данных
                    for (const auto& value : row) {
                        // Добавляем значение в строку данных
                        rowData.push_back(value);
                    }
                    // Добавляем строку данных в вектор чанка
                    chunkData.push_back(rowData);
                }
                // Если чанк загружен успешно, возвращаем true
                return true;
            }
            // Если чанк не содержит данных, выводим предупреждение
            oCmdInfo.warning("The chunk '" + requiredChunk + "' does not contain the 'data' ");
        }
        // Если чанк не найден, возвращаем false
        return false;
    }

    // Если файл коррумпирован, выводим предупреждение
    oCmdInfo.warning("File is corrupted, there is no line 'chunks'");
    // Возвращаем false, если файл коррумпирован
    return false;
}


/**
 * Сохраняет чанк в буфере.
 *
 * @param chunkX Координата X чанка.
 * @param chunkY Координата Y чанка.
 * @param data Данные чанка.
 * @param chunkSize Размер чанка.
 */
void cMap::saveChunk(const int chunkX, const int chunkY, const std::vector<std::vector<int>>& data, const int chunkSize) {
    // Создаём имя чанка на основе его координат
    const std::string chunkName = std::to_string(chunkX) + "<>" + std::to_string(chunkY);

    // Сохраняем данные чанка в буфере
    chunkBuffer[chunkName] = data;

    // Если размер буфера превышает 300, сохраняем данные в файл
    if (size(chunkBuffer) > 300) save();
}

/**
 * Сохраняет текущий мир в файл.
 *
 * Если мир новый, создает новый файл и записывает в него начальные данные.
 * Если мир уже существует, открывает файл, считывает данные и обновляет их.
 *
 * @note Функция также очищает буфер чанков после сохранения.
 */
void cMap::save() {
    // Проверяем, открыт ли файл мира для записи
    if (!fileWorldIsOpen) {
        // Если мир новый, создаем новый файл и записываем в него начальные данные
        if (m_bNewWorld) {
            fileWorld.open("worlds/"+strNameFileWorld, std::ofstream::app);
            if(fileWorld.is_open()) {
                oCmdInfo.info("New file " + strNameFileWorld + " is open for writing");
                jsonSave = json::object();
                jsonSave["seed"] = m_uiSeed;
                jsonSave["chunks"] = json::object();
                m_bNewWorld = false;
                fileWorld.close();
            } else {
                oCmdInfo.error("Cannot open file " + strNameFileWorld + " for writing");
            }
        }
        // Открываем файл мира для чтения и записи
        fileWorld.open("worlds/"+strNameFileWorld);
        if (fileWorld.is_open()) {
            // Если файл не пустой, считываем данные из него
            if (fileWorld.peek() != std::ifstream::traits_type::eof()) {
                oCmdInfo.info("The file " + strNameFileWorld + " is open for writing");
                fileWorld >> jsonSave;
            }
            fileWorld.close();
            fileWorldIsOpen = true;
        } else {
            oCmdInfo.error("Cannot open file " + strNameFileWorld + " for writing");
            return;
        }
    }

    // Начинаем процесс сохранения мира
    oCmdInfo.info("Start save");

    // Обходим все чанки в буфере и записываем их в файл
    for (const auto& [chunkName, data] : chunkBuffer) {
        json chunkData = json::array();
        for (const auto& row : data) {
            chunkData.push_back(row);
        }
        jsonSave["chunks"][chunkName]["data"] = chunkData;
    }

    // Обновляем позицию игрока в файле
    jsonSave["player"]["pos"]["x"] = PlayerPos.x;
    jsonSave["player"]["pos"]["y"] = PlayerPos.y;

    // Открываем файл мира для записи и сбрасываем его содержимое
    fileWorld.open("worlds/" + strNameFileWorld, std::ofstream::out | std::ofstream::trunc);
    if (fileWorld.is_open()) {
        // Записываем данные в файл
        fileWorld << jsonSave.dump(4);
        fileWorld.close();
        oCmdInfo.info("Saved");
    } else {
        oCmdInfo.error("Cannot open file" + strNameFileWorld + " to save changes");
    }

    // Очищаем буфер чанков после сохранения
    chunkBuffer.clear();
    loadingChunksFromFile();
}

//
// Получение слоя
//
int cMap::getLayer(const int x, const int y, const int layer) const {
    if (layer == 0) return LayerGround[y][x];
    if (layer == 1) return LayerOdj[y][x];
    return -1;
}

//
// Установка слоя
//
void cMap::setLayer(const int x, const int y, const int layer, const int value) {
    if (layer == 0) {
        LayerGround[y][x] = value;
    } else if (layer == 1) {
        LayerOdj[y][x] = value;
    }
}

/**
 * Инициализация созданного мира.
 *
 * Эта функция инициализирует карту, загружая чанки из файла и тайлсет,
 * а также запускает поток для обработки чанков.
 *
 * @param iDistanceView Расстояние видимости
 * @param strNameFileMap Имя файла карты
 * @param window Окно SFML
 */
void cMap::init(const int iDistanceView, const std::string& strNameFileMap, const sf::RenderWindow& window) {
    // Установка расстояния видимости
    this->m_iDistanceView = iDistanceView;

    // Формирование имени файла мира
    this->strNameFileWorld = strNameFileMap + ".json";

    // Загрузка чанков из файла
    loadingChunksFromFile();

    // Загрузка тайлсета
    loadTileset();

    // Запуск потока, если он еще не запущен
    if (!isThreadRunning) {
        isThreadRunning = true;
        isThreadStarted = true;
        thread.launch();
    } else {
        isThreadStarted = true;
    }
}
/**
 * Инициализация нового мира
 *
 * @param iDistanceView - расстояние видимости
 * @param strNameFileMap - имя файла карты
 * @param window - окно SFML
 * @param seed - зерно генератора случайных чисел
 */
void cMap::init(const int iDistanceView, const std::string& strNameFileMap, const sf::RenderWindow& window, const unsigned int seed) {
    // Устанавливаем зерно генератора случайных чисел
    this->m_uiSeed = seed;
    // Устанавливаем расстояние видимости
    this->m_iDistanceView = iDistanceView;
    // Формируем имя файла карты
    this->strNameFileWorld = strNameFileMap + ".json";
    // Устанавливаем флаг нового мира
    m_bNewWorld = true;
    // Сохраняем данные
    save();
    // Загружаем чанки из файла
    loadingChunksFromFile();
    // Загружаем тайлсет
    loadTileset();

    // Запускаем поток, если он еще не запущен
    if (!isThreadRunning) {
        isThreadRunning = true;
        isThreadStarted = true;
        thread.launch();
    } else {
        isThreadStarted = true;
    }
}

void cMap::chunkLoadUnloadThread() {
    while (isThreadRunning) {
        if (isThreadStarted) {
            std::lock_guard<std::mutex> lock(MUTEX);
            loadChunksAroundPlayer();
            unloadDistantChunks();
        }
    }
}


//
// Загрузка тайл сета
//
void cMap::loadTileset() {
    try {
        oCmdInfo.info("Loading tile set");

        fileInput.open("Maps/map2/3.json");
        fileInput >> objJson;
        fileInput.close();

        imageHeight = static_cast<int>(objJson["imageheight"]);
        imageWidth = static_cast<int>(objJson["imagewidth"]);

        if (!texture.loadFromFile("Maps/map2/3.png")) {

            return;
        }

        sprite.setTexture(texture);
        oCmdInfo.info("Tile set loaded successfully");
    }
    catch (const nlohmann::json::exception& error) {
        std::cerr << "ERROR: Failed to load tileset " << error.what() << std::endl;
    }
}


//
// Проверка коллизий
//
int cMap::collision(const sf::Vector2f playerPos, const sf::Vector2f playerSize, const sf::Vector2f bias) const {
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

//
// Загрузка чанков ко круг игрока в зависимости от его позиии и дальности прорисовки
//
void cMap::loadChunksAroundPlayer() {
    const int playerChunkX = static_cast<int>(PlayerPos.x) / (chunkSize * g_dTileSize);
    const int playerChunkY = static_cast<int>(PlayerPos.y) / (chunkSize * g_dTileSize);

    for (int y = -m_iDistanceView; y <= m_iDistanceView; ++y) {
        for (int x = -m_iDistanceView; x <= m_iDistanceView; ++x) {
            int chunkX = playerChunkX + x;
            int chunkY = playerChunkY + y;

            if (auto chunkKey = std::make_pair(chunkX, chunkY); chunks.find(chunkKey) == chunks.end()) {
                const std::string name = std::to_string(chunkX) + "<>" + std::to_string(chunkY);
                //std::cout << name << std::endl;
                if (std::vector<std::vector<int>> temp = chunks[chunkKey].data; checkingDownloadedChunks(name, temp)) {
                    chunks[chunkKey].data = temp;
                    saveChunk(chunkX, chunkY, chunks[chunkKey].data, chunkSize);
                } else {
                    if (ComGenerateChunkRUN) {
                        chunks[chunkKey] = generateChunk(chunkX, chunkY, m_uiSeed, chunkSize);
                        saveChunk(chunkX, chunkY, chunks[chunkKey].data, chunkSize);
                    }
                }
            }
        }
    }
}

//
// Выгрузка чанков
//
void cMap::unloadDistantChunks() {

    const int playerChunkX = static_cast<int>(PlayerPos.x) / (chunkSize * g_dTileSize);
    const int playerChunkY = static_cast<int>(PlayerPos.y) / (chunkSize * g_dTileSize);

    for (auto it = chunks.begin(); it != chunks.end();) {
        const int chunkX = it->first.first;

        if (const int chunkY = it->first.second; abs(chunkX - playerChunkX) > m_iDistanceView || abs(chunkY - playerChunkY) > m_iDistanceView) {
            auto chunkKey = std::make_pair(chunkX, chunkY);
            saveChunk(chunkX, chunkY, chunks[chunkKey].data, chunkSize);
            it = chunks.erase(it);
        } else {
            ++it;
        }
    }
}

//
// Рисовка
//
void cMap::draw(sf::RenderWindow &window, const sf::Vector2f playerPos, sf::Vector2f view, const int chunkSize) {
    // Lock the mutex while modifying chunks
   // std::lock_guard<std::mutex> lock(chunkMutex);
    this->PlayerPos = playerPos;
    //loadChunksAroundPlayer(playerPos, chunkSize);

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
                int i = 0;
                for (const auto& el1 : chunks[chunkKey].data) {
                    int j = 0;
                    for (const auto& el2 : el1) {
                        if (const int tileValue = el2; tileValue != 0) {
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
                        j++;
                    }
                    i++;
                }
                //для деревьев и друких структур для них чанк состоит 8 на 8 а для тайлов 16 на 16
                // нужно чтобы у текстур были свои не зависимые кординаты на карте

                // Размеры дерева
                int treeXSize = 48;
                int treeYSize = 64;
                // Создаем генератор случайных чисел
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> disX(-treeXSize / 4, treeXSize / 4);  // Смещение по X
                std::uniform_int_distribution<> disY(-treeYSize / 4, treeYSize / 4);  // Смещение по Y

                i = 0;
                for (const auto& el1 : chunks[chunkKey].dataObjects) {
                    int j = 0;
                    for (const auto& el2 : el1) {
                        if (const int tileValue = el2; tileValue != 0) {


                            // Координаты для дерева в мире
                            float X = static_cast<float>(j + chunkX * (chunkSize)) * treeXSize;
                            float Y = static_cast<float>(i + chunkY * (chunkSize)) * treeYSize;

                            // Добавляем случайные смещения
                            X += disX(gen);
                            Y += disY(gen);

                            // Координаты дерева на текстуре (верхний левый угол: 192,0 и нижний правый угол: 240,64)
                            const float tileX = 192; // X координата на текстуре
                            const float tileY = 0;   // Y координата на текстуре

                            // Добавляем вершины для дерева, учитывая размеры (48x64)
                            vertices.append(sf::Vertex(sf::Vector2f(X - roundedOffsetX, Y - roundedOffsetY), sf::Vector2f(tileX, tileY)));
                            vertices.append(sf::Vertex(sf::Vector2f(X + treeXSize - roundedOffsetX, Y - roundedOffsetY), sf::Vector2f(tileX + treeXSize, tileY)));
                            vertices.append(sf::Vertex(sf::Vector2f(X + treeXSize - roundedOffsetX, Y + treeYSize - roundedOffsetY), sf::Vector2f(tileX + treeXSize, tileY + treeYSize)));
                            vertices.append(sf::Vertex(sf::Vector2f(X - roundedOffsetX, Y + treeYSize - roundedOffsetY), sf::Vector2f(tileX, tileY + treeYSize)));
                        }
                        j++;
                    }
                    i++;
                }
            }
        }
    }

    sf::RenderStates states;
    states.texture = &texture;
    window.draw(vertices, states);
    //unloadDistantChunks(playerPos, chunkSize);
}

//
// Создание мира
//
void cMap::createWorld() {

}