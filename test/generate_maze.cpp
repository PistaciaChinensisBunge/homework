#include "Maze.h"
#include "MazeGenerator.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <output_file> [rows] [cols]" << std::endl;
        return 1;
    }

    int rows = 20;
    int cols = 20;

    if (argc >= 4) {
        rows = std::stoi(argv[2]);
        cols = std::stoi(argv[3]);
    }

    Maze maze;
    MazeGenerator generator;

    std::cout << "Generating " << rows << "x" << cols << " maze..." << std::endl;
    generator.generateMaze(maze, rows, cols);

    std::string filename = argv[1];
    if (maze.saveToFile(filename)) {
        std::cout << "Maze saved to " << filename << std::endl;
    } else {
        std::cerr << "Failed to save maze" << std::endl;
        return 1;
    }

    return 0;
}
