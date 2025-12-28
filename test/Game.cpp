#include "Game.h"
#include <raylib.h>

Game::Game() : pathFinder(nullptr), state(STATE_START),
               currentPathType(PATH_DFS) {}

bool Game::init(const std::string& mazeFile, const std::string& resourcePath) {
    if (!maze.loadFromFile(mazeFile)) {
        return false;
    }

    pathFinder = PathFinder(&maze);

    renderer.initWindow(maze);

    if (!renderer.loadTextures(resourcePath)) {
        return false;
    }

    paths[PATH_DFS] = pathFinder.findPathDFS();
    paths[PATH_BFS] = pathFinder.findPathBFS();
    paths[PATH_DIJKSTRA] = pathFinder.findPathDijkstra();
    paths[PATH_DIJKSTRA_ONE_LAVA] = pathFinder.findPathDijkstraOneLava();

    player.reset(maze.getStartPos());

    return true;
}

void Game::handleInput() {
    if (state == STATE_START) {
        if (IsKeyPressed(KEY_SPACE)) {
            state = STATE_VIEWING;
        }
    } else if (state == STATE_VIEWING) {
        if (IsKeyPressed(KEY_ONE)) {
            currentPathType = PATH_DFS;
        } else if (IsKeyPressed(KEY_TWO)) {
            currentPathType = PATH_BFS;
        } else if (IsKeyPressed(KEY_THREE)) {
            currentPathType = PATH_DIJKSTRA;
        } else if (IsKeyPressed(KEY_FOUR)) {
            currentPathType = PATH_DIJKSTRA_ONE_LAVA;
        } else if (IsKeyPressed(KEY_ENTER)) {
            state = STATE_PLAYING;
            player.reset(maze.getStartPos());
        }
    } else if (state == STATE_PLAYING) {
        float deltaTime = GetFrameTime();

        if (player.canMove(deltaTime)) {
            if (IsKeyDown(KEY_UP)) {
                player.move(-1, 0, maze);
            } else if (IsKeyDown(KEY_DOWN)) {
                player.move(1, 0, maze);
            } else if (IsKeyDown(KEY_LEFT)) {
                player.move(0, -1, maze);
            } else if (IsKeyDown(KEY_RIGHT)) {
                player.move(0, 1, maze);
            }
        }

        if (player.isOnEnd(maze)) {
            state = STATE_WIN;
        } else if (player.getLavaStepCount() >= 2) {
            state = STATE_LOSE;
        }
    } else if (state == STATE_WIN || state == STATE_LOSE) {
        if (IsKeyPressed(KEY_R)) {
            player.reset(maze.getStartPos());
            state = STATE_PLAYING;
        } else if (IsKeyPressed(KEY_V)) {
            state = STATE_VIEWING;
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        renderer.closeWindow();
    }
}

void Game::update(float deltaTime) {
    // Future updates can be added here
}

void Game::render() {
    renderer.beginDrawing();
    renderer.clearBackground(RAYWHITE);

    renderer.drawMaze(maze);

    if (state == STATE_VIEWING) {
        Color pathColor;
        std::string pathName;

        switch (currentPathType) {
            case PATH_DFS:
                pathColor = RED;
                pathName = "DFS Path";
                break;
            case PATH_BFS:
                pathColor = BLUE;
                pathName = "BFS Path";
                break;
            case PATH_DIJKSTRA:
                pathColor = GREEN;
                pathName = "Dijkstra Shortest Path";
                break;
            case PATH_DIJKSTRA_ONE_LAVA:
                pathColor = PURPLE;
                pathName = "Dijkstra with One Lava";
                break;
        }

        renderer.drawPath(paths[currentPathType], pathColor);
        renderer.drawGameState(pathName,
            "1:DFS  2:BFS  3:Dijkstra  4:OneLava  ENTER:Play");

    } else if (state == STATE_PLAYING) {
        renderer.drawPlayer(player.getPosition());

        std::string info = "Use arrow keys to move";
        if (player.getLavaStepCount() > 0) {
            info = "Lava steps: " + std::to_string(player.getLavaStepCount()) + "/2";
        }

        renderer.drawGameState("Playing", info);

    } else if (state == STATE_START) {
        renderer.drawGameState("Maze Game", "Press SPACE to start");

    } else if (state == STATE_WIN) {
        renderer.drawPlayer(player.getPosition());
        renderer.drawGameState("You Win!", "R:Restart  V:View Paths");

    } else if (state == STATE_LOSE) {
        renderer.drawPlayer(player.getPosition());
        renderer.drawGameState("Game Over", "R:Restart  V:View Paths");
    }

    renderer.endDrawing();
}

void Game::run() {
    while (!WindowShouldClose()) {
        handleInput();
        update(GetFrameTime());
        render();
    }

    renderer.closeWindow();
}
