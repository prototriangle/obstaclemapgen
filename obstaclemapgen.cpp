#include <utility>

#include "obstaclemapgen.h"

#include <iostream>

#include <utility>
#include <iomanip>
#include <sstream>

#include "utility.h"
#include "randomnumbergenerator.h"
#include "randomobstaclegenerator.h"
#include "map_file_writer.h"

namespace ObstacleMapGen {

    void GenerateRandomObstacles(MapParams params,
                                 std::vector<Obstacle> &obs,
                                 unsigned int lowerObsCount,
                                 unsigned int upperObsCount) {
        auto boxCount = static_cast<unsigned int>(g_rng.UniformInt(lowerObsCount, upperObsCount));
        RandomObstacleGenerator obstacleGenerator = RandomObstacleGenerator(boxCount, params);
        int attempts = obstacleGenerator.Generate();
        obs = std::move(obstacleGenerator.obstacles);
    }

    Matrix2D CreateObstacleMap(MapParams params, std::vector<Obstacle> &os, unsigned int x_res, unsigned int y_res) {
        Matrix2D map;

        const double x_range_bot = params.minX;
        const double y_range_bot = params.minZ;
        const double x_range_top = params.maxX;
        const double y_range_top = params.maxZ;

        const double x_incr = (x_range_top - x_range_bot) / (double) x_res;
        const double y_incr = (y_range_top - y_range_bot) / (double) y_res;

        Matrix2D z_vals(y_res);
        double x = x_range_bot;
        double y = y_range_bot;
        for (unsigned int i = 0; i < y_res; ++i) {
            y = y_range_bot + y_incr * i + y_incr / 2;
            for (unsigned int j = 0; j < x_res; ++j) {
                x = x_range_bot + x_incr * j + x_incr / 2;
                Vec2D point(x, y);
                if (PointInAnyObstacle(point, os)) {
                    z_vals[i].push_back(1);
                } else {
                    z_vals[i].push_back(0);
                }
            }
        }

        map = std::move(z_vals);
        return map;
    }

    void CreateAndWriteOutObstacleMaps(MapParams params,
                                       std::string directory, unsigned int n,
                                       unsigned int x_res, unsigned int y_res,
                                       unsigned int lowerObsCount, unsigned int upperObsCount) {
        MapFileWriter writer(std::move(directory));
        for (unsigned int i = 0; i < n; ++i) {
            std::vector<Obstacle> obs;
            GenerateRandomObstacles(params, obs, lowerObsCount, upperObsCount);
            auto map = CreateObstacleMap(params, obs, x_res, y_res);
            writer.WriteMapToFile(map, obs, params);
        }
        writer.GenerateMetadataFile();
    }

}

