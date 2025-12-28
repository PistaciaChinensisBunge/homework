#include "PathFinder.h"
#include <queue>
#include <map>
#include <limits>
#include <algorithm>

PathFinder::PathFinder(const Maze* mazePtr) : maze(mazePtr) {}

bool PathFinder::dfsHelper(int row, int col, std::vector<Position>& path,
                           std::set<std::pair<int,int>>& visited) {
    if (row == maze->getEndPos().row && col == maze->getEndPos().col) {
        path.push_back({row, col});
        return true;
    }

    visited.insert({row, col});
    path.push_back({row, col});

    int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (int i = 0; i < 4; i++) {
        int newRow = row + dirs[i][0];
        int newCol = col + dirs[i][1];

        if (maze->isWalkable(newRow, newCol) &&
            visited.find({newRow, newCol}) == visited.end()) {
            if (dfsHelper(newRow, newCol, path, visited)) {
                return true;
            }
        }
    }

    path.pop_back();
    return false;
}

std::vector<Position> PathFinder::findPathDFS() {
    std::vector<Position> path;
    std::set<std::pair<int,int>> visited;

    Position start = maze->getStartPos();
    dfsHelper(start.row, start.col, path, visited);

    return path;
}

std::vector<Position> PathFinder::findPathBFS() {
    std::vector<Position> path;
    Position start = maze->getStartPos();
    Position end = maze->getEndPos();

    std::queue<Position> queue;
    std::map<std::pair<int,int>, Position> parent;
    std::set<std::pair<int,int>> visited;

    queue.push(start);
    visited.insert({start.row, start.col});
    parent[{start.row, start.col}] = {-1, -1};

    int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    bool found = false;
    while (!queue.empty() && !found) {
        Position current = queue.front();
        queue.pop();

        if (current == end) {
            found = true;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int newRow = current.row + dirs[i][0];
            int newCol = current.col + dirs[i][1];

            if (maze->isWalkable(newRow, newCol) &&
                visited.find({newRow, newCol}) == visited.end()) {

                Position next = {newRow, newCol};
                queue.push(next);
                visited.insert({newRow, newCol});
                parent[{newRow, newCol}] = current;
            }
        }
    }

    if (found) {
        Position current = end;
        while (current.row != -1) {
            path.push_back(current);
            current = parent[{current.row, current.col}];
        }
        std::reverse(path.begin(), path.end());
    }

    return path;
}

std::vector<Position> PathFinder::findPathDijkstra() {
    std::vector<Position> path;
    Position start = maze->getStartPos();
    Position end = maze->getEndPos();

    std::map<std::pair<int,int>, int> dist;
    std::map<std::pair<int,int>, Position> parent;
    std::set<std::pair<int, std::pair<int,int>>> pq;

    for (int i = 0; i < maze->getRows(); i++) {
        for (int j = 0; j < maze->getCols(); j++) {
            dist[{i, j}] = std::numeric_limits<int>::max();
        }
    }

    dist[{start.row, start.col}] = 0;
    pq.insert({0, {start.row, start.col}});
    parent[{start.row, start.col}] = {-1, -1};

    int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while (!pq.empty()) {
        auto current = *pq.begin();
        pq.erase(pq.begin());

        int currentDist = current.first;
        int row = current.second.first;
        int col = current.second.second;

        if (row == end.row && col == end.col) {
            break;
        }

        for (int i = 0; i < 4; i++) {
            int newRow = row + dirs[i][0];
            int newCol = col + dirs[i][1];

            if (maze->isWalkable(newRow, newCol)) {
                int cost = maze->getCost(row, col, newRow, newCol);
                int newDist = currentDist + cost;

                if (newDist < dist[{newRow, newCol}]) {
                    pq.erase({dist[{newRow, newCol}], {newRow, newCol}});
                    dist[{newRow, newCol}] = newDist;
                    pq.insert({newDist, {newRow, newCol}});
                    parent[{newRow, newCol}] = {row, col};
                }
            }
        }
    }

    if (dist[{end.row, end.col}] != std::numeric_limits<int>::max()) {
        Position current = end;
        while (current.row != -1) {
            path.push_back(current);
            current = parent[{current.row, current.col}];
        }
        std::reverse(path.begin(), path.end());
    }

    return path;
}

std::vector<Position> PathFinder::findPathDijkstraOneLava() {
    std::vector<Position> path;
    Position start = maze->getStartPos();
    Position end = maze->getEndPos();

    std::map<std::pair<std::pair<int,int>, bool>, int> dist;
    std::map<std::pair<std::pair<int,int>, bool>, std::pair<Position, bool>> parent;
    std::set<std::pair<int, std::pair<std::pair<int,int>, bool>>> pq;

    for (int i = 0; i < maze->getRows(); i++) {
        for (int j = 0; j < maze->getCols(); j++) {
            dist[{{i, j}, false}] = std::numeric_limits<int>::max();
            dist[{{i, j}, true}] = std::numeric_limits<int>::max();
        }
    }

    dist[{{start.row, start.col}, false}] = 0;
    pq.insert({0, {{start.row, start.col}, false}});
    parent[{{start.row, start.col}, false}] = {{-1, -1}, false};

    int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while (!pq.empty()) {
        auto current = *pq.begin();
        pq.erase(pq.begin());

        int currentDist = current.first;
        int row = current.second.first.first;
        int col = current.second.first.second;
        bool usedLava = current.second.second;

        if (row == end.row && col == end.col) {
            break;
        }

        for (int i = 0; i < 4; i++) {
            int newRow = row + dirs[i][0];
            int newCol = col + dirs[i][1];

            if (!maze->isValid(newRow, newCol)) continue;

            int cell = maze->getCell(newRow, newCol);
            if (cell == WALL) continue;

            int cost;
            bool newUsedLava = usedLava;

            if (cell == LAVA) {
                if (!usedLava) {
                    cost = 1;
                    newUsedLava = true;
                } else {
                    cost = 1000;
                }
            } else if (cell == GRASS) {
                cost = 3;
            } else {
                cost = 1;
            }

            int newDist = currentDist + cost;

            if (newDist < dist[{{newRow, newCol}, newUsedLava}]) {
                pq.erase({dist[{{newRow, newCol}, newUsedLava}], {{newRow, newCol}, newUsedLava}});
                dist[{{newRow, newCol}, newUsedLava}] = newDist;
                pq.insert({newDist, {{newRow, newCol}, newUsedLava}});
                parent[{{newRow, newCol}, newUsedLava}] = {{row, col}, usedLava};
            }
        }
    }

    int bestDist = std::min(dist[{{end.row, end.col}, false}],
                            dist[{{end.row, end.col}, true}]);

    if (bestDist != std::numeric_limits<int>::max()) {
        bool usedLava = (dist[{{end.row, end.col}, true}] < dist[{{end.row, end.col}, false}]);

        auto current = std::make_pair(end, usedLava);
        while (current.first.row != -1) {
            path.push_back(current.first);
            current = parent[{{current.first.row, current.first.col}, current.second}];
        }
        std::reverse(path.begin(), path.end());
    }

    return path;
}
