#include "../include/PerlinNoise.hpp"

// Конструктор по умолчанию использует случайное зерно
PerlinNoise::PerlinNoise(const unsigned int seed) {
    reseed(seed);
}

// Пересев (перегенерация) случайных значений
void PerlinNoise::reseed(const unsigned int seed) {
    p.resize(256);
    std::iota(p.begin(), p.end(), 0);
    std::default_random_engine engine(seed);
    std::shuffle(p.begin(), p.end(), engine);
    p.insert(p.end(), p.begin(), p.end());
}

// Шум в 1D
double PerlinNoise::noise(double x) const {
    int X = static_cast<int>(std::floor(x)) & 255;
    x -= std::floor(x);
    double u = fade(x);
    return lerp(u, grad(p[X], x), grad(p[X + 1], x - 1.0));
}

// Шум в 2D
double PerlinNoise::noise(double x, double y) const {
    const int X = static_cast<int>(std::floor(x)) & 255;
    const int Y = static_cast<int>(std::floor(y)) & 255;
    x -= std::floor(x);
    y -= std::floor(y);
    const double u = fade(x);
    const double v = fade(y);
    const int A = p[X] + Y;
    const int AB = p[A + 1];
    const int AA = p[A];
    const int B = p[X + 1] + Y;
    const int BB = p[B + 1];
    const int BA = p[B];
    return lerp(v, lerp(u, grad(p[AA], x, y), grad(p[BA], x - 1, y)),
                lerp(u, grad(p[AB], x, y - 1), grad(p[BB], x - 1, y - 1)));
}

// Шум в 2D с октавами
double PerlinNoise::octave2D_01(const double x, const double y, const int octaves, const double persistence) const {
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

double PerlinNoise::fade(const double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(const double t, const double a, const double b) {
    return a + t * (b - a);
}

double PerlinNoise::grad(const int hash, const double x, const double y) {
    const int h = hash & 7;
    const double u = h < 4 ? x : y;
    const double v = h < 4 ? y : x;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0 * v : 2.0 * v);
}

double PerlinNoise::grad(const int hash, const double x) {
    return (hash & 1) ? -x : x;
}
