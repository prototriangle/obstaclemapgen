#ifndef MAP_FILE_READER_H
#define MAP_FILE_READER_H

#include "utility.h"

namespace ObstacleMapGen {

    class MapFileReader {

    public:

        static const char LOAD_ENV = 1;
        static const char LOAD_OBSTACLES = 2;
        static const char LOAD_MATRIX = 4;
        static const unsigned long MAX_ITERATIONS = 300;

        bool ReadFromFile(Matrix2D &mat, std::vector<Obstacle> &obstacles, MapParams &env, std::string filename, char flags = LOAD_ENV | LOAD_OBSTACLES | LOAD_MATRIX);

        inline void read_env(std::ifstream &in, MapParams &env);

        inline void read_obstacles(std::ifstream &in, std::vector<Obstacle> &obstacles);

        inline void read_matrix(std::ifstream &in, Matrix2D &mat);
    };

}
#endif
