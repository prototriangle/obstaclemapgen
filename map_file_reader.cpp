#include <iostream>
#include <fstream>
#include "map_file_reader.h"


namespace ObstacleMapGen {

    bool MapFileReader::ReadFromFile(Matrix2D &mat, std::vector<Obstacle> &obstacles, MapParams &env,
                                     std::string filename, char flags) {
        std::ifstream file(filename);
        if (file.is_open() && file.good()) {
            if (flags & LOAD_ENV) read_env(file, env);
            if (flags & LOAD_OBSTACLES) read_obstacles(file, obstacles);
            if (flags & LOAD_MATRIX) read_matrix(file, mat);
            file.close();
        } else {
            std::cout << "Unable to open file " << filename << std::endl;
        }
        return false;
    }

    void MapFileReader::read_env(std::ifstream &in, MapParams &env) {
        in >> env.maxX;
        in >> env.minX;
        in >> env.maxZ;
        in >> env.minZ;
        in >> env.maxS;
        in >> env.minS;
        in >> env.height;
        in >> env.spacing;
        in >> env.padding;
        in >> env.margin;
        std::cout << env.maxX << " "
                  << env.minX << " "
                  << env.maxZ << " "
                  << env.minZ << " "
                  << env.maxS << " "
                  << env.minS << " "
                  << env.height << " "
                  << env.spacing << " "
                  << env.padding << " "
                  << env.margin << std::endl;
    }

    void MapFileReader::read_obstacles(std::ifstream &in, std::vector<Obstacle> &obstacles) {
        char c = static_cast<char>(in.peek());
        unsigned long i = 0;
        while (c == '~' || c == '\n') {
            in.ignore();
            c = static_cast<char>(in.peek());
            if (++i > 5) return;
        }
        i = 0;
        while (c != '~' && !in.eof()) {
            Obstacle o;
            in >> o.originX;
            in >> o.originZ;
            in >> o.sizeX;
            in >> o.sizeZ;
            in >> o.rot;
            o.calculateCorners();
            obstacles.push_back(o);
            in.ignore(256, '\n');
            c = static_cast<char>(in.peek());
            if (++i > MAX_ITERATIONS) return;
        }
    }

    void MapFileReader::read_matrix(std::ifstream &in, Matrix2D &mat) {
        char c = static_cast<char>(in.peek());
        unsigned long i = 0;
        while (c == '~' || c == '\n') {
            in.ignore();
            c = static_cast<char>(in.peek());
            if (++i > MAX_ITERATIONS) return;
        }
        i = 0;
        while (c != '~' && c != EOF) {
            std::vector<double> row;
            unsigned long j = 0;
            while (c != '\n' && c != EOF) {
                double d;
                in >> d;
                row.push_back(d);
                c = static_cast<char>(in.peek());
                if (++j > MAX_ITERATIONS) return;
            }
            mat.push_back(row);
            in.ignore(256, '\n');
            c = static_cast<char>(in.peek());
            if (++i > MAX_ITERATIONS) return;
        }
    }
}