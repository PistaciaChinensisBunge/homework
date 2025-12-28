#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Maze.h"
#include <vector>
#include <set>

enum PathType {
    PATH_DFS,
    PATH_BFS,
    PATH_DIJKSTRA,
    PATH_DIJKSTRA_ONE_LAVA
};

class PathFinder {
private:
    const Maze* maze;

    bool dfsHelper(int row, int col, std::vector<Position>& path,
                   std::set<std::pair<int,int>>& visited);

public:
    PathFinder(const Maze* mazePtr);

    std::vector<Position> findPathDFS();
    std::vector<Position> findPathBFS();
    std::vector<Position> findPathDijkstra();
    std::vector<Position> findPathDijkstraOneLava();
};

#endif
