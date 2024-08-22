#include "../include/Chunk.hpp"

void Chunk::generate() {
    cells.resize(SIZE, std::vector<int>(SIZE, 0));
    srand(seed + chunkX * 73856093 + chunkY * 19349663);
    initializeCells();

    for (int step = 0; step < 5; ++step) {
        std::vector<std::vector<int>> newCells = cells;
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                int waterCount = 0;
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        if (i + dx >= 0 && i + dx < SIZE && j + dy >= 0 && j + dy < SIZE) {
                            if (cells[i + dx][j + dy] == 159) {
                                waterCount++;
                            }
                        }
                    }
                }
                if (waterCount > 4) {
                    newCells[i][j] = 159;
                } else {
                    newCells[i][j] = 47;
                }
            }
        }
        cells = newCells;
    }
}

void Chunk::initializeCells() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            cells[i][j] = (rand() % 100 < 45) ? 159 : 47; // Примерная вероятность воды
        }
    }
}

const std::vector<std::vector<int>>& Chunk::getCells() const {
    return cells;
}
