#ifndef OBSTACLE_MAP_GENRANDOMNUMBERGENERATOR_H
#define OBSTACLE_MAP_GENRANDOMNUMBERGENERATOR_H

#include <random>

namespace ObstacleMapGen {

    class RandomNumberGenerator {

        std::default_random_engine generator;

    public:

        RandomNumberGenerator() = default;

        explicit RandomNumberGenerator(unsigned long seed);

        float UniformFloat(float lower, float upper);

        double UniformDouble(double lower, double upper);

        int UniformInt(int lower, int upper);
    };

    extern RandomNumberGenerator g_rng;

}
#endif //OBSTACLE_MAP_GEN_RANDOMNUMBERGENERATOR_H
