#ifndef TETRIS_CLONE_MANAGER_H_
#define TETRIS_CLONE_MANAGER_H_

#include <stdbool.h>

#include "tetromino.h"

void InitManager(void);
void EmptyAllBlocks(void);
bool CantSpawnBlock(void);
void Update(void);
void RequestRotate(void);
void RequestMoveLeft(void);
void RequestMoveRight(void);
struct TetrominoBlock** GetUpdatedBlocks(int* lengthOfBlocksUpdated);
void CleanupManager(void);

#endif
