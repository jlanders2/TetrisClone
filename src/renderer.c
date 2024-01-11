#include <stdio.h>
#include "raylib.h"

#include "renderer.h"

#define PNG_RES(name) "resources/" #name ".png"
#define BLOCK_SIZE 32

Texture2D yellowTexture;
Texture2D blueTexture;
Texture2D lightBlueTexture;
Texture2D redTexture;
Texture2D purpleTexture;
Texture2D greenTexture;
Texture2D orangeTexture;
Texture2D emptyTexture;
Texture2D gameOverTexture;

void InitRenderer(void) {
    yellowTexture = LoadTexture(PNG_RES(yellow));
    blueTexture = LoadTexture(PNG_RES(blue));
    lightBlueTexture = LoadTexture(PNG_RES(light-blue));
    redTexture = LoadTexture(PNG_RES(red));
    purpleTexture = LoadTexture(PNG_RES(purple));
    greenTexture = LoadTexture(PNG_RES(green));
    orangeTexture = LoadTexture(PNG_RES(orange));
    emptyTexture = LoadTexture(PNG_RES(black));
    gameOverTexture = LoadTexture(PNG_RES(gameOver));
}

Texture2D* GetTextureFromTetrominoType(enum TetrominoType type) {
    switch (type) {
        case O_TYPE:
        return &yellowTexture;
        case I_TYPE:
        return &lightBlueTexture;
        case T_TYPE:
        return &purpleTexture;
        case J_TYPE:
        return &blueTexture;
        case L_TYPE:
        return &orangeTexture;
        case S_TYPE:
        return &greenTexture;
        case Z_TYPE:
        return &redTexture;
        default:
        return &emptyTexture;
    }
}

void RenderTetrominoBlock(struct TetrominoBlock blockToRender) {
    Texture2D* textureToRender = GetTextureFromTetrominoType(blockToRender.type);
    DrawTexture(*textureToRender, blockToRender.x * BLOCK_SIZE, blockToRender.y * BLOCK_SIZE, WHITE); 
}

void RenderBlocks(struct TetrominoBlock** updatedBlocks, int length) {
    BeginDrawing();
    
    while(length--) {
        RenderTetrominoBlock(*(*updatedBlocks));
        updatedBlocks++;
    }

    EndDrawing();
}

void RenderGameOver(void) {
    BeginDrawing();

    ClearBackground(BLACK);
    DrawText("Game Over", SCREEN_WIDTH / 2 - MeasureText("Game Over", 24) / 2, 200, 24, RED);
    DrawText("Score: N/A", SCREEN_WIDTH / 2 - MeasureText("Score: N/A", 18) / 2, 232, 18, RED);

    EndDrawing();
}