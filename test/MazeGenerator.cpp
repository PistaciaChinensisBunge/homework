#include "MazeGenerator.h"
#include <chrono>
#include <algorithm>

MazeGenerator::MazeGenerator() {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng.seed(seed);
}

void MazeGenerator::generatePath(Maze& maze, int row, int col,
                                 std::vector<std::vector<bool>>& visited) {
    visited[row][col] = true;
    maze.setCell(row, col, FLOOR);

    int dirs[4][2] = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};

    std::shuffle(std::begin(dirs), std::end(dirs), rng);

    for (int i = 0; i < 4; i++) {
        int newRow = row + dirs[i][0];
        int newCol = col + dirs[i][1];

        if (maze.isValid(newRow, newCol) && !visited[newRow][newCol]) {
            int wallRow = row + dirs[i][0] / 2;
            int wallCol = col + dirs[i][1] / 2;

            maze.setCell(wallRow, wallCol, FLOOR);
            generatePath(maze, newRow, newCol, visited);
        }
    }
}

void MazeGenerator::addSpecialTiles(Maze& maze, int grassCount, int lavaCount) {
    std::vector<Position> floorTiles;

    for (int i = 1; i < maze.getRows() - 1; i++) {
        for (int j = 1; j < maze.getCols() - 1; j++) {
            if (maze.getCell(i, j) == FLOOR) {
                Position pos = {i, j};
                if (pos != maze.getStartPos() && pos != maze.getEndPos()) {
                    floorTiles.push_back(pos);
                }
            }
        }
    }

    std::shuffle(floorTiles.begin(), floorTiles.end(), rng);

    for (int i = 0; i < grassCount && i < (int)floorTiles.size(); i++) {
        maze.setCell(floorTiles[i].row, floorTiles[i].col, GRASS);
    }

    for (int i = grassCount; i < grassCount + lavaCount && i < (int)floorTiles.size(); i++) {
        maze.setCell(floorTiles[i].row, floorTiles[i].col, LAVA);
    }
}

void MazeGenerator::generateMaze(Maze& maze, int rows, int cols) {
    generateMaze(maze, rows, cols, rows * cols / 20, rows * cols / 40);
}

void MazeGenerator::generateMaze(Maze& maze, int rows, int cols,
                                 int grassCount, int lavaCount) {
    maze.init(rows, cols);

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
                maze.setCell(i, j, WALL);
            } else {
                maze.setCell(i, j, WALL);
            }
        }
    }

    int startRow = 1;
    int startCol = 1;
    generatePath(maze, startRow, startCol, visited);

    maze.setCell(startRow, startCol, START);
    maze.setStartPos({startRow, startCol});

    maze.setCell(rows - 2, cols - 2, END);
    maze.setEndPos({rows - 2, cols - 2});

    addSpecialTiles(maze, grassCount, lavaCount);
}
