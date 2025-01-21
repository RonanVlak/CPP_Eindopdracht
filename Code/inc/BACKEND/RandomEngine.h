#ifndef RANDOMENGINE_H
#define RANDOMENGINE_H

#include <random>

class RandomEngine
{
public:
    static std::mt19937& getInstance();
    static int getRandomInt(int min, int max);

private:
    RandomEngine() = default;
    ~RandomEngine() = default;
    RandomEngine(const RandomEngine&) = delete;
    RandomEngine& operator=(const RandomEngine&) = delete;
};

#endif // RANDOMENGINE_H