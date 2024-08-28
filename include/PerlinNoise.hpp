#pragma once

#include <vector>
#include <numeric>
#include <random>
#include <algorithm>
#include <cmath>

class PerlinNoise {
public:
    // Конструктор по умолчанию использует случайное зерно
    explicit PerlinNoise(unsigned int seed = std::default_random_engine::default_seed);

    // Пересев (перегенерация) случайных значений
    void reseed(unsigned int seed);

    // Шум в 1D
    [[nodiscard]] double noise(double x) const;

    // Шум в 2D
    [[nodiscard]] double noise(double x, double y) const;

    // Шум в 2D с октавами
    [[nodiscard]] double octave2D_01(double x, double y, int octaves, double persistence) const;

private:
    std::vector<int> p;

    static double fade(double t);

    static double lerp(double t, double a, double b);

    static double grad(int hash, double x, double y);
    static double grad(int hash, double x);
};

