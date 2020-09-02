#include <tuple>
#include "randomobstaclegenerator.h"
#include "randomnumbergenerator.h"
#include "utility.h"

namespace ObstacleMapGen {

    RandomObstacleGenerator::RandomObstacleGenerator(unsigned long obstacleCount, MapParams params)
            : obstacleCount(obstacleCount), params(params) {
        obstacles.resize(0);
    }

    int RandomObstacleGenerator::Generate(unsigned long maxRetries) {
        printf("Starting obstacle generation...\n");
        fflush(stdout);
        int retries = 0;
        int backtrack_count = 0;
        while (obstacles.size() < obstacleCount) {
            obstacles.emplace_back(RandomX(), RandomZ(), RandomSize(), RandomSize(), RandomRot());
            if (AnyObstacleIntersects() || OutOfBounds(obstacles.back()) || NotSparseEnough()) {
                obstacles.pop_back();
                retries++;
            }
            if (retries >= maxRetries) {
                obstacles.pop_back();
                retries = 0;
                break;
                backtrack_count++;
            }

            if (backtrack_count >= maxRetries) {
                obstacles.pop_back();
                break;
            }
//        printf("Obstacles count: %lu\n", obstacles.size());
        }
        printf(" Done\n");
        return backtrack_count;
    }

    float RandomObstacleGenerator::RandomX() {
        return g_rng.UniformFloat(params.minX, params.maxX);
    }

    float RandomObstacleGenerator::RandomZ() {
        return g_rng.UniformFloat(params.minZ, params.maxZ);
    }

    float RandomObstacleGenerator::RandomSize() {
        return g_rng.UniformFloat(params.minS, params.maxS);
    }

    float RandomObstacleGenerator::RandomRot() {
        return g_rng.UniformFloat(0.0f, 0.0f);
    }

    bool RandomObstacleGenerator::AnyObstacleIntersects() {
        auto first = obstacles.begin();
        auto last = obstacles.end();
        for (; first != last; ++first) {
            for (auto next = std::next(first); next != last; ++next) {
                bool intersects = ObstaclesIntersect(*first, *next);
                if (intersects) return intersects;
            }
        }
        return false;
    }

    bool RandomObstacleGenerator::OutOfBounds(Obstacle &obstacle) {
        bool result = false;
        result |= obstacle.c1.x > params.maxX - params.padding || obstacle.c1.x < params.minX + params.padding;
        result |= obstacle.c2.x > params.maxX - params.padding || obstacle.c2.x < params.minX + params.padding;
        result |= obstacle.c3.x > params.maxX - params.padding || obstacle.c3.x < params.minX + params.padding;
        result |= obstacle.c4.x > params.maxX - params.padding || obstacle.c4.x < params.minX + params.padding;
        result |= obstacle.c1.y > params.maxZ - params.padding || obstacle.c1.y < params.minZ + params.padding;
        result |= obstacle.c2.y > params.maxZ - params.padding || obstacle.c2.y < params.minZ + params.padding;
        result |= obstacle.c3.y > params.maxZ - params.padding || obstacle.c3.y < params.minZ + params.padding;
        result |= obstacle.c4.y > params.maxZ - params.padding || obstacle.c4.y < params.minZ + params.padding;
        return result;
    }

    bool RandomObstacleGenerator::NotSparseEnough() {
        auto first = obstacles.begin();
        auto last = obstacles.end();
        double additional_spacing = 0.;
        for (; first != last; ++first) {
            for (auto next = std::next(first); next != last; ++next) {
                bool r = ObstacleCornerMinDistnaceLessThan(*first, *next, params.spacing + additional_spacing);
                r |= MinDistanceBetweenObstacles(*first, *next) < (params.spacing + additional_spacing);
                if (r) return true;
            }
        }
        return false;
    }

}

