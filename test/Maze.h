#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>
#include <utility>

enum CellType {
    START = -1,
    END = -2,
    FLOOR = 0,
    WALL = 1,
    GRASS = 2,
    LAVA = 3
};

struct Position {
    int row;
    int col;

    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

class Maze {
private:
    int rows;
    int cols;
    std::vector<std::vector<int>> grid;
    Position startPos;
    Position endPos;

public:
    Maze();
    Maze(int rows, int cols);
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;

    void init(int rows, int cols);

    int getRows() const { return rows; }
    int getCols() const { return cols; }
    int getCell(int row, int col) const;
    void setCell(int row, int col, int value);

    Position getStartPos() const { return startPos; }
    Position getEndPos() const { return endPos; }
    void setStartPos(const Position& pos) { startPos = pos; }
    void setEndPos(const Position& pos) { endPos = pos; }

    bool isValid(int row, int col) const;
    bool isWalkable(int row, int col) const;

    int getCost(int fromRow, int fromCol, int toRow, int toCol) const;
};

#endif
