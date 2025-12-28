#include "Maze.h"
#include <fstream>
#include <sstream>
#include <iostream>

Maze::Maze() : rows(0), cols(0), startPos({-1, -1}), endPos({-1, -1}) {}

Maze::Maze(int r, int c) : rows(r), cols(c), startPos({-1, -1}), endPos({-1, -1}) {
    grid.resize(rows, std::vector<int>(cols, WALL));
}

void Maze::init(int r, int c) {
    rows = r;
    cols = c;
    grid.clear();
    grid.resize(rows, std::vector<int>(cols, WALL));
    startPos = {-1, -1};
    endPos = {-1, -1};
}

bool Maze::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return false;
    }

    file >> rows >> cols;
    if (rows <= 0 || cols <= 0) {
        std::cerr << "Invalid maze dimensions" << std::endl;
        return false;
    }

    grid.resize(rows, std::vector<int>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            file >> grid[i][j];

            if (grid[i][j] == START) {
                startPos = {i, j};
            } else if (grid[i][j] == END) {
                endPos = {i, j};
            }
        }
    }

    file.close();

    if (startPos.row == -1 || endPos.row == -1) {
        std::cerr << "Start or end position not found" << std::endl;
        return false;
    }

    return true;
}

bool Maze::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot create file: " << filename << std::endl;
        return false;
    }

    file << rows << " " << cols << std::endl;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            file << grid[i][j];
            if (j < cols - 1) file << " ";
        }
        file << std::endl;
    }

    file.close();
    return true;
}

int Maze::getCell(int row, int col) const {
    if (!isValid(row, col)) {
        return WALL;
    }
    return grid[row][col];
}

void Maze::setCell(int row, int col, int value) {
    if (isValid(row, col)) {
        grid[row][col] = value;
    }
}

bool Maze::isValid(int row, int col) const {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

bool Maze::isWalkable(int row, int col) const {
    if (!isValid(row, col)) return false;
    int cell = grid[row][col];
    return cell != WALL;
}

int Maze::getCost(int fromRow, int fromCol, int toRow, int toCol) const {
    if (!isValid(toRow, toCol)) return 1000000;

    int cell = grid[toRow][toCol];

    if (cell == WALL) return 1000000;
    if (cell == LAVA) return 1000;
    if (cell == GRASS) return 3;

    return 1;
}
