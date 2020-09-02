#ifndef OBSTACLE_MAP_GEN_LIBRARY_H
#define OBSTACLE_MAP_GEN_LIBRARY_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "utility.h"

namespace ObstacleMapGen {

    void GenerateRandomObstacles(MapParams params,
                                 std::vector<Obstacle> &obs,
                                 unsigned int lowerObsCount = 5,
                                 unsigned int upperObsCount = 9);

    Matrix2D CreateObstacleMap(MapParams params,
                               std::vector<Obstacle> &os,
                               unsigned int x_res = 50,
                               unsigned int y_res = 50);

    void CreateAndWriteOutObstacleMaps(MapParams params,
                                       std::string directory,
                                       unsigned int n = 1000,
                                       unsigned int x_res = 50,
                                       unsigned int y_res = 50,
                                       unsigned int lowerObsCount = 5,
                                       unsigned int upperObsCount = 9);
}

#endif