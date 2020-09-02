cmake_minimum_required(VERSION 2.8)
project(obstaclemapgen)

#set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

file(GLOB HEADERS obstaclemapgen.h utility.h map_file_reader.h map_file_writer.h)

add_library(obstaclemapgen SHARED
        utility.h
        obstaclemapgen.cpp
        obstaclemapgen.h
        randomnumbergenerator.h
        randomobstaclegenerator.h
        randomnumbergenerator.cpp
        randomobstaclegenerator.cpp
        map_file_reader.h
        map_file_reader.cpp
        map_file_writer.h
        map_file_writer.cpp)

set_target_properties(obstaclemapgen PROPERTIES
        VERSION 1
        SOVERSION 1
        PUBLIC_HEADER "${HEADERS}")

install(TARGETS obstaclemapgen
        LIBRARY DESTINATION /usr/local/lib/
        PUBLIC_HEADER DESTINATION /usr/local/include/obstaclemapgen)
