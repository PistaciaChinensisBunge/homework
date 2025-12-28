#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include "Maze.h"
#include <random>

class MazeGenerator {
private:
    std::mt19937 rng;

    void generatePath(Maze& maze, int row, int col,
                     std::vector<std::vector<bool>>& visited);
    void addSpecialTiles(Maze& maze, int grassCount, int lavaCount);

public:
    MazeGenerator();
    void generateMaze(Maze& maze, int rows, int cols);
    void generateMaze(Maze& maze, int rows, int cols,
                     int grassCount, int lavaCount);
};

#endif
