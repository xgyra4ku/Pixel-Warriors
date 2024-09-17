#include "../include/PerlinNoise.hpp"

//
// Конструктор по умолчанию использует случайное зерно
//
PerlinNoise::PerlinNoise(const unsigned int seed) {
    reseed(seed);
}

//
// Пересев (перегенерация) случайных значений
//
void PerlinNoise::reseed(const unsigned int seed) {
    m_viPerl.resize(256);
    std::iota(m_viPerl.begin(), m_viPerl.end(), 0);
    std::default_random_engine engine(seed);
    std::shuffle(m_viPerl.begin(), m_viPerl.end(), engine);
    m_viPerl.insert(m_viPerl.end(), m_viPerl.begin(), m_viPerl.end());
}

//
// Шум в 1D
//
double PerlinNoise::noise(double x) const {
    int X = static_cast<int>(std::floor(x)) & 255;
    x -= std::floor(x);
    double u = fade(x);
    return lerp(u, grad(m_viPerl[X], x), grad(m_viPerl[X + 1], x - 1.0));
}

//
// Шум в 2D
//
double PerlinNoise::noise(double x, double y) const {
    const int X = static_cast<int>(std::floor(x)) & 255;
    const int Y = static_cast<int>(std::floor(y)) & 255;
    x -= std::floor(x);
    y -= std::floor(y);
    const double u = fade(x);
    const double v = fade(y);
    const int A = m_viPerl[X] + Y;
    const int AB = m_viPerl[A + 1];
    const int AA = m_viPerl[A];
    const int B = m_viPerl[X + 1] + Y;
    const int BB = m_viPerl[B + 1];
    const int BA = m_viPerl[B];
    return lerp(v, lerp(u, grad(m_viPerl[AA], x, y), grad(m_viPerl[BA], x - 1, y)),
                lerp(u, grad(m_viPerl[AB], x, y - 1), grad(m_viPerl[BB], x - 1, y - 1)));
}

//
// Шум в 2D с октавами
//
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

//
// функция
//
double PerlinNoise::fade(const double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

//
// функция
//
double PerlinNoise::lerp(const double t, const double a, const double b) {
    return a + t * (b - a);
}

//
// функция
//
double PerlinNoise::grad(const int hash, const double x, const double y) {
    const int h = hash & 7;
    const double u = h < 4 ? x : y;
    const double v = h < 4 ? y : x;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0 * v : 2.0 * v);
}

//
// функция
//
double PerlinNoise::grad(const int hash, const double x) {
    return (hash & 1) ? -x : x;
}
