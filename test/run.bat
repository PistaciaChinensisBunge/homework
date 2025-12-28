@echo off
echo Compiling Maze Game in VSCode...

cd /d "%~dp0"
C:\mingw64\bin\g++.exe -o MazeGame.exe ^
    main.cpp ^
    Game.cpp ^
    Maze.cpp ^
    MazeGenerator.cpp ^
    PathFinder.cpp ^
    Player.cpp ^
    Renderer.cpp ^
    -IC:\raylib\include ^
    -LC:\raylib\lib ^
    -lraylib ^
    -lopengl32 ^
    -lgdi32 ^
    -lwinmm ^
    -static

if exist MazeGame.exe (
    echo.
    echo Build successful!
    echo Running game...
    echo.
    MazeGame.exe
) else (
    echo Build failed!
)

pause