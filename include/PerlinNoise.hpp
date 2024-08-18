#ifndef PERLIN_NOISE_HPP
#define PERLIN_NOISE_HPP

#include <vector>
#include <numeric>
#include <random>
#include <algorithm>
#include <cmath>

class PerlinNoise {
public:
    // Конструктор по умолчанию использует случайное зерно
    explicit PerlinNoise(unsigned int seed = std::default_random_engine::default_seed) {
        reseed(seed);
    }

    // Пересев (перегенерация) случайных значений
    void reseed(unsigned int seed) {
        p.resize(256);
        std::iota(p.begin(), p.end(), 0);
        std::default_random_engine engine(seed);
        std::shuffle(p.begin(), p.end(), engine);
        p.insert(p.end(), p.begin(), p.end());
    }

    // Шум в 1D
    double noise(double x) const {
        int X = static_cast<int>(std::floor(x)) & 255;
        x -= std::floor(x);
        double u = fade(x);
        return lerp(u, grad(p[X], x), grad(p[X + 1], x - 1.0));
    }

    // Шум в 2D
    double noise(double x, double y) const {
        int X = static_cast<int>(std::floor(x)) & 255;
        int Y = static_cast<int>(std::floor(y)) & 255;
        x -= std::floor(x);
        y -= std::floor(y);
        double u = fade(x);
        double v = fade(y);
        int A = p[X] + Y, AA = p[A], AB = p[A + 1];
        int B = p[X + 1] + Y, BA = p[B], BB = p[B + 1];
        return lerp(v, lerp(u, grad(p[AA], x, y), grad(p[BA], x - 1, y)),
                      lerp(u, grad(p[AB], x, y - 1), grad(p[BB], x - 1, y - 1)));
    }

    // Шум в 2D с октавами
    double octave2D_01(double x, double y, int octaves, double persistence) const {
        double total = 0.0;
        double frequency = 1.0;
        double amplitude = 1.0;
        double maxValue = 0.0;

        for (int i = 0; i < octaves; ++i) {
            total += noise(x * frequency, y * frequency) * amplitude;
            maxValue += amplitude;
            amplitude *= persistence;
            frequency *= 2.0;
        }

        return total / maxValue;
    }

private:
    std::vector<int> p;

    static double fade(double t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    static double lerp(double t, double a, double b) {
        return a + t * (b - a);
    }

    static double grad(int hash, double x, double y) {
        int h = hash & 7;
        double u = h < 4 ? x : y;
        double v = h < 4 ? y : x;
        return ((h & 1) ? -u : u) + ((h & 2) ? -2.0 * v : 2.0 * v);
    }

    static double grad(int hash, double x) {
        return (hash & 1) ? -x : x;
    }
};

#endif // PERLIN_NOISE_HPP
