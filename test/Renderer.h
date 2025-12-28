#ifndef RENDERER_H
#define RENDERER_H

#include <raylib.h>
#include "Maze.h"
#include <vector>
#include <map>
#include <string>

class Renderer {
private:
    int tileSize;
    int screenWidth;
    int screenHeight;

    std::map<int, Texture2D> tileTextures;
    Texture2D characterTexture;

    bool texturesLoaded;

public:
    Renderer(int tileSizeValue = 35);
    ~Renderer();

    bool loadTextures(const std::string& resourcePath);
    void unloadTextures();

    void initWindow(const Maze& maze);
    void closeWindow();

    void drawMaze(const Maze& maze);
    void drawPath(const std::vector<Position>& path, Color color);
    void drawPlayer(const Position& pos);
    void drawText(const std::string& text, int x, int y, int fontSize, Color color);
    void drawGameState(const std::string& state, const std::string& info);

    void beginDrawing();
    void endDrawing();
    void clearBackground(Color color);

    int getTileSize() const { return tileSize; }
};

#endif
