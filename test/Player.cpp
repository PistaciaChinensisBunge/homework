#include "Player.h"

Player::Player()
    : position({0, 0}), lavaStepCount(0), moveTimer(0.0f),
      moveDelay(0.0f), normalSpeed(0.15f), grassSpeed(0.45f) {}

void Player::reset(const Position& startPos) {
    position = startPos;
    lavaStepCount = 0;
    moveTimer = 0.0f;
    moveDelay = 0.0f;
}

bool Player::canMove(float deltaTime) {
    moveTimer += deltaTime;
    if (moveTimer >= moveDelay) {
        moveTimer = 0.0f;
        return true;
    }
    return false;
}

void Player::move(int drow, int dcol, const Maze& maze) {
    int newRow = position.row + drow;
    int newCol = position.col + dcol;

    if (maze.isWalkable(newRow, newCol)) {
        position.row = newRow;
        position.col = newCol;

        int cell = maze.getCell(newRow, newCol);
        updateMoveDelay(cell);

        if (cell == LAVA) {
            lavaStepCount++;
        }
    }
}

void Player::updateMoveDelay(int cellType) {
    if (cellType == GRASS) {
        moveDelay = grassSpeed;
    } else {
        moveDelay = normalSpeed;
    }
}

bool Player::isOnLava(const Maze& maze) const {
    return maze.getCell(position.row, position.col) == LAVA;
}

bool Player::isOnEnd(const Maze& maze) const {
    return position == maze.getEndPos();
}
