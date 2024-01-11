#ifndef TETRIS_CLONE_RENDERER_H_
#define TETRIS_CLONE_RENDERER_H_
#include "raylib.h"

#include "tetromino.h"

#define SCREEN_HEIGHT 640
#define SCREEN_WIDTH 320

void r_initialize(void);
void r_render_blocks(struct TetrominoBlock* *blocks, int length);
void r_render_game_over(int score);

void _r_render_block(struct TetrominoBlock blockToRender);
Texture2D *_r_get_texture_by_tetromino_type(enum TetrominoType type);

#endif
