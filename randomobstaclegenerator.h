#ifndef OBSTACLE_MAP_GEN_RANDOMOBSTACLEGENERATOR_H
#define OBSTACLE_MAP_GEN_RANDOMOBSTACLEGENERATOR_H

#include <vector>
#include <random>
#include "utility.h"

namespace ObstacleMapGen {

    class RandomObstacleGenerator {
        unsigned long obstacleCount;
        MapParams params;

    public:
        std::vector<Obstacle> obstacles;

        RandomObstacleGenerator(unsigned long obstacleCount, MapParams params);


        int Generate(unsigned long maxRetries = 300L);

    private:

        float RandomX();

        float RandomZ();

        float RandomSize();

        float RandomRot();

        bool AnyObstacleIntersects();

        bool OutOfBounds(Obstacle &obstacle);

        bool NotSparseEnough();
    };
}
#endif //OBSTACLE_MAP_GEN_RANDOMOBSTACLEGENERATOR_H
