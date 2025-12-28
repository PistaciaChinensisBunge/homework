#ifndef PLAYER_H
#define PLAYER_H

#include "Maze.h"

class Player {
private:
    Position position;
    int lavaStepCount;
    float moveTimer;
    float moveDelay;
    float normalSpeed;
    float grassSpeed;

public:
    Player();

    void reset(const Position& startPos);
    void setPosition(const Position& pos) { position = pos; }
    Position getPosition() const { return position; }

    int getLavaStepCount() const { return lavaStepCount; }
    void incrementLavaStep() { lavaStepCount++; }

    bool canMove(float deltaTime);
    void move(int drow, int dcol, const Maze& maze);
    void updateMoveDelay(int cellType);

    bool isOnLava(const Maze& maze) const;
    bool isOnEnd(const Maze& maze) const;
};

#endif
