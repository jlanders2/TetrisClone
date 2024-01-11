#include "renderer.h"

#define RES_PNG_FILE(name) "resources/" #name ".png"
#define TEXTURE_SIZE 32

Texture2D _yellow_texture;
Texture2D _blue_texture;
Texture2D _light_blue_texture;
Texture2D _red_texture;
Texture2D _purple_texture;
Texture2D _green_texture;
Texture2D _orange_texture;
Texture2D _empty_texture;

void r_initialize(void) {
    _yellow_texture = LoadTexture(RES_PNG_FILE(yellow));
    _blue_texture = LoadTexture(RES_PNG_FILE(blue));
    _light_blue_texture = LoadTexture(RES_PNG_FILE(light-blue));
    _red_texture = LoadTexture(RES_PNG_FILE(red));
    _purple_texture = LoadTexture(RES_PNG_FILE(purple));
    _green_texture = LoadTexture(RES_PNG_FILE(green));
    _orange_texture = LoadTexture(RES_PNG_FILE(orange));
    _empty_texture = LoadTexture(RES_PNG_FILE(black));
}

void r_render_blocks(struct TetrominoBlock* *blocks, int length) {
    BeginDrawing();
    while(length--) {
        _r_render_block(*(*blocks));
        blocks++;
    }
    EndDrawing();
}

void r_render_game_over(int score) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Game Over", SCREEN_WIDTH / 2 - MeasureText("Game Over", 24) / 2, 200, 24, RED);
    char* score_message = TextFormat("Score: %d", score);
    DrawText(score_message, SCREEN_WIDTH / 2 - MeasureText(score_message, 18) / 2, 232, 18, RED);
    // Draw a play again button
    EndDrawing();
}

void _r_render_block(struct TetrominoBlock blockToRender) {
    Texture2D* textureToRender = _r_get_texture_by_tetromino_type(blockToRender.type);
    DrawTexture(*textureToRender, blockToRender.x * TEXTURE_SIZE, blockToRender.y * TEXTURE_SIZE, WHITE); 
}

Texture2D* _r_get_texture_by_tetromino_type(enum TetrominoType type) {
    switch (type) {
        case TT_O:
        return &_yellow_texture;
        case TT_I:
        return &_light_blue_texture;
        case TT_T:
        return &_purple_texture;
        case TT_J:
        return &_blue_texture;
        case TT_L:
        return &_orange_texture;
        case TT_S:
        return &_green_texture;
        case TT_Z:
        return &_red_texture;
        default:
        return &_empty_texture;
    }
}