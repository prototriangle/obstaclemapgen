#include "randomnumbergenerator.h"

namespace ObstacleMapGen {

    RandomNumberGenerator g_rng;

    float RandomNumberGenerator::UniformFloat(float lower, float upper) {
        std::uniform_real_distribution<float> distributionX{lower, upper};
        return distributionX(generator);
    }

    double RandomNumberGenerator::UniformDouble(double lower, double upper) {
        std::uniform_real_distribution<double> distributionX{lower, upper};
        return distributionX(generator);
    }

    int RandomNumberGenerator::UniformInt(int lower, int upper) {
        std::uniform_int_distribution<int> distribution{lower, upper};
        return distribution(generator);
    }

    RandomNumberGenerator::RandomNumberGenerator(unsigned long seed) : generator(seed) {}

}