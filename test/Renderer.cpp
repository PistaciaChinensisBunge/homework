#include "Renderer.h"
#include <iostream>

Renderer::Renderer(int tileSizeValue)
    : tileSize(tileSizeValue), screenWidth(800), screenHeight(600),
      texturesLoaded(false) {}

Renderer::~Renderer() {
    if (texturesLoaded) {
        unloadTextures();
    }
}

bool Renderer::loadTextures(const std::string& resourcePath) {
    Image floorImg = LoadImage((resourcePath + "floor.png").c_str());
    tileTextures[FLOOR] = LoadTextureFromImage(floorImg);
    UnloadImage(floorImg);

    Image wallImg = LoadImage((resourcePath + "wall.png").c_str());
    tileTextures[WALL] = LoadTextureFromImage(wallImg);
    UnloadImage(wallImg);

    Image grassImg = LoadImage((resourcePath + "grass.png").c_str());
    tileTextures[GRASS] = LoadTextureFromImage(grassImg);
    UnloadImage(grassImg);

    Image lavaImg = LoadImage((resourcePath + "lava.png").c_str());
    tileTextures[LAVA] = LoadTextureFromImage(lavaImg);
    UnloadImage(lavaImg);

    Image startImg = LoadImage((resourcePath + "start.png").c_str());
    tileTextures[START] = LoadTextureFromImage(startImg);
    UnloadImage(startImg);

    Image endImg = LoadImage((resourcePath + "end.png").c_str());
    tileTextures[END] = LoadTextureFromImage(endImg);
    UnloadImage(endImg);

    Image charImg = LoadImage((resourcePath + "character.png").c_str());
    characterTexture = LoadTextureFromImage(charImg);
    UnloadImage(charImg);

    texturesLoaded = true;
    return true;
}

void Renderer::unloadTextures() {
    for (auto& pair : tileTextures) {
        UnloadTexture(pair.second);
    }
    UnloadTexture(characterTexture);
    texturesLoaded = false;
}

void Renderer::initWindow(const Maze& maze) {
    screenWidth = maze.getCols() * tileSize;
    screenHeight = maze.getRows() * tileSize + 100;
    InitWindow(screenWidth, screenHeight, "Maze Game");
    SetTargetFPS(60);
}

void Renderer::closeWindow() {
    CloseWindow();
}

void Renderer::drawMaze(const Maze& maze) {
    for (int i = 0; i < maze.getRows(); i++) {
        for (int j = 0; j < maze.getCols(); j++) {
            int cell = maze.getCell(i, j);

            int x = j * tileSize;
            int y = i * tileSize;

            if (tileTextures.find(cell) != tileTextures.end()) {
                Rectangle source = {0, 0, (float)tileTextures[cell].width,
                                   (float)tileTextures[cell].height};
                Rectangle dest = {(float)x, (float)y, (float)tileSize, (float)tileSize};
                DrawTexturePro(tileTextures[cell], source, dest, {0, 0}, 0, WHITE);
            } else {
                DrawRectangle(x, y, tileSize, tileSize, GRAY);
            }
        }
    }
}

void Renderer::drawPath(const std::vector<Position>& path, Color color) {
    for (size_t i = 0; i < path.size(); i++) {
        int x = path[i].col * tileSize + tileSize / 2;
        int y = path[i].row * tileSize + tileSize / 2;

        DrawCircle(x, y, tileSize / 6, color);

        if (i > 0) {
            int prevX = path[i-1].col * tileSize + tileSize / 2;
            int prevY = path[i-1].row * tileSize + tileSize / 2;
            DrawLine(prevX, prevY, x, y, color);
        }
    }
}

void Renderer::drawPlayer(const Position& pos) {
    int x = pos.col * tileSize;
    int y = pos.row * tileSize;

    Rectangle source = {0, 0, (float)characterTexture.width,
                       (float)characterTexture.height};
    Rectangle dest = {(float)x, (float)y, (float)tileSize, (float)tileSize};
    DrawTexturePro(characterTexture, source, dest, {0, 0}, 0, WHITE);
}

void Renderer::drawText(const std::string& text, int x, int y,
                        int fontSize, Color color) {
    DrawText(text.c_str(), x, y, fontSize, color);
}

void Renderer::drawGameState(const std::string& state, const std::string& info) {
    int yOffset = screenHeight - 90;
    DrawRectangle(0, yOffset, screenWidth, 90, Fade(BLACK, 0.7f));

    DrawText(state.c_str(), 10, yOffset + 10, 30, WHITE);
    DrawText(info.c_str(), 10, yOffset + 50, 20, LIGHTGRAY);
}

void Renderer::beginDrawing() {
    BeginDrawing();
}

void Renderer::endDrawing() {
    EndDrawing();
}

void Renderer::clearBackground(Color color) {
    ClearBackground(color);
}
