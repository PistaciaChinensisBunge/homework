#ifndef GAME_H
#define GAME_H

#include "Maze.h"
#include "PathFinder.h"
#include "Renderer.h"
#include "Player.h"
#include <map>

enum GameState {
    STATE_START,
    STATE_VIEWING,
    STATE_PLAYING,
    STATE_WIN,
    STATE_LOSE
};

class Game {
private:
    Maze maze;
    PathFinder pathFinder;
    Renderer renderer;
    Player player;

    GameState state;
    PathType currentPathType;

    std::map<PathType, std::vector<Position>> paths;

    void handleInput();
    void update(float deltaTime);
    void render();

public:
    Game();
    bool init(const std::string& mazeFile, const std::string& resourcePath);
    void run();
};

#endif
