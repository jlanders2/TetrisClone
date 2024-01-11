#ifndef TETRIS_CLONE_RENDERER_H_
#define TETRIS_CLONE_RENDERER_H_

#include "tetromino.h"

#define SCREEN_HEIGHT 640
#define SCREEN_WIDTH 320

void InitRenderer(void);
void RenderBlocks(struct TetrominoBlock** updatedBlocks, int length);
void RenderGameOver(void);

#endif
