#ifndef MAP_FILE_WRITER_H
#define MAP_FILE_WRITER_H

#include <iostream>
#include <fstream>

#include "utility.h"

namespace ObstacleMapGen {

    class MapFileWriter {
        struct MetaDatum {
            unsigned int index;
            unsigned long obstacle_count;
            unsigned long seed;
            std::string path;

            friend std::ostream &operator<<(std::ostream &stream, const MetaDatum &m) {
                stream << m.index << "\t"
                       << m.obstacle_count << "\t"
                       << m.seed << "\t"
                       << m.path;
                return stream;
            }
        };

        std::string directory;
        std::ofstream current_file;
        unsigned int index = 1;
        std::vector<MetaDatum> metadata;

    public:
        explicit MapFileWriter(std::string directory = "./");

        bool WriteMapToFile(const Matrix2D &mat, std::string filename, std::vector<Obstacle> obstacles, MapParams env);

        bool WriteMapToFile(const Matrix2D &mat);

        bool WriteMapToFile(const Matrix2D &mat, std::vector<Obstacle> obstacles, MapParams env);

        bool WriteMapToFile(const Matrix2D &mat, std::vector<Obstacle> obstacles, unsigned long seed, MapParams env);

        bool GenerateMetadataFile();

    };

}
#endif
