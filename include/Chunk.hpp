#pragma once
#include <vector>
#include <cstdlib>
#include <SFML/Graphics.hpp>

class Chunk {
public:
    static const int SIZE = 16;

    void generate();
    const std::vector<std::vector<int>>& getCells() const;

private:
    int chunkX, chunkY;
    unsigned int seed;
    std::vector<std::vector<int>> cells;

    void initializeCells();
};
