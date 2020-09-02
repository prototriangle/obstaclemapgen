#include <utility>
#include <iomanip>
#include <sstream>

#include "map_file_writer.h"

namespace ObstacleMapGen {

    MapFileWriter::MapFileWriter(std::string directory) : directory(std::move(directory)) {}


    bool MapFileWriter::WriteMapToFile(const Matrix2D &mat, std::string filename, std::vector<Obstacle> obstacles,
                                       MapParams env) {
        std::string path = directory + filename;
        std::cout << path << std::endl;
        current_file.open(path);
        if (current_file.is_open()) {
            std::cout << "Writing to file " << directory << filename << std::endl;
            current_file << env.maxX << " "
                         << env.minX << " "
                         << env.maxZ << " "
                         << env.minZ << " "
                         << env.maxS << " "
                         << env.minS << " "
                         << env.height << " "
                         << env.spacing << " "
                         << env.padding << " "
                         << env.margin << std::endl;
            current_file << "~~~" << std::endl;
            for (auto &obstacle : obstacles) {
                current_file << obstacle << std::endl;
            }
            current_file << "~~~" << std::endl;
            for (auto &row : mat) {
                for (unsigned int i = 0; i < row.size(); ++i) {
                    current_file << row[i];
                    if (i + 1 < row.size())
                        current_file << "\t";
                }
                current_file << std::endl;
            }
            current_file.close();
            metadata.push_back({index, obstacles.size(), 0, path});
            ++index;
            return true;
        } else {
            std::cout << "Unable to open file " << directory << filename << std::endl;
            return false;
        }
    }


    bool MapFileWriter::WriteMapToFile(const Matrix2D &mat) {
        return false;
    }

    bool MapFileWriter::WriteMapToFile(const Matrix2D &mat, std::vector<Obstacle> obstacles, MapParams env) {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
//    oss << std::put_time(&tm, "%d-%m_%H-%M_") << index << ".map";
        oss << "obstacles_" << index << ".map";
        auto filename = oss.str();

        return WriteMapToFile(mat, filename, std::move(obstacles), env);
    }

    bool MapFileWriter::WriteMapToFile(const Matrix2D &mat, std::vector<Obstacle> obstacles, unsigned long seed,
                                       MapParams env) {
        metadata.push_back({index, obstacles.size(), seed});
        return WriteMapToFile(mat, std::move(obstacles), env);
    }

    bool MapFileWriter::GenerateMetadataFile() {
        std::string filename = "metadata.dat";
        std::string path = directory + filename;
        std::cout << path << std::endl;
        current_file.open(path, std::ofstream::out);
        if (current_file.is_open()) {
            std::cout << "Writing to file " << directory << filename << std::endl;
            for (auto &m : metadata) {
                current_file << m << std::endl;
            }
            current_file.close();
            return true;
        } else {
            std::cout << "Unable to open file " << directory << filename << std::endl;
            return false;
        }
    }
}