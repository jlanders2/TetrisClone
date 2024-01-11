#include <stdio.h>

#include "raylib.h"
#include "manager.h"
#include "renderer.h"
#include "tetromino.h"

#define TARGET_FPS 60

void EmptyGameBoard(void) {
    EmptyAllBlocks();

    int numberOfUpdatedBlocks = 0;
    struct TetrominoBlock** updatedBlocks = GetUpdatedBlocks(&numberOfUpdatedBlocks);
}

void InitRaylib(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris Clone");

    SetTargetFPS(TARGET_FPS);
}

void Init(void) {
    InitRaylib();
    InitManager();
    InitRenderer();
    EmptyGameBoard();
}

void ProcessInput(void) {
    if (IsKeyPressed(KEY_SPACE)) {
        RequestRotate();
    }
    if (IsKeyDown(KEY_A)) {
        RequestMoveLeft();
    } else if (IsKeyDown(KEY_D)) {
        RequestMoveRight();
    }
}

void Loop(void) {
    int tickRate = 0; // Guaranteed a better way to do this XD
    while(!WindowShouldClose()) {
        while(!CantSpawnBlock()) {
            ProcessInput();
            if (tickRate == 0) {
                Update();
                tickRate = 5;
            }
            
            int numberOfUpdatedBlocks = 0;
            struct TetrominoBlock** updatedBlocks = GetUpdatedBlocks(&numberOfUpdatedBlocks);

            RenderBlocks(updatedBlocks, numberOfUpdatedBlocks);
            tickRate--;
        }
        // TODO present Game over message and score
        RenderGameOver();
    }
}

void Cleanup() {
  CleanupManager();
  CloseWindow();
}

void start(void) {
    Init();
    Loop();
    Cleanup();
}