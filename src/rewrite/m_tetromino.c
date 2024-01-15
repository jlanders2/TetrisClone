#include "m_tetromino.h"
#include "m_block.h"
#include <limits.h>

/* Blocks are gauranteed to be in order top left to bottom right */
block_t *_tetromino_blocks[BLOCKS_WITHIN_A_TETROMINO];
int registered_flag = 0;

void M_T_Update_Tetromino(void) { M_T_Tetromino_Fall(); }

void M_T_Tetromino_Fall(void) {
  int i;
  if (registered_flag && M_B_Can_Move_Blocks_Down(_tetromino_blocks)) {
    for (i = BLOCKS_WITHIN_A_TETROMINO - 1; i >= 0; i--) {
      _tetromino_blocks[i] = M_B_Move_Block_Down(_tetromino_blocks[i]);
    }
  }
}

void M_T_Register_Falling_Blocks(block_t *blocks[BLOCKS_WITHIN_A_TETROMINO]) {
  int i;
  if (!registered_flag) {
    registered_flag = 1;
  }
  for (i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
    _tetromino_blocks[i] = blocks[i];
  }
}

void M_T_Move_Tetromino_Left(void) {
  int i;
  printf("test");
  if (registered_flag && M_B_Can_Move_Blocks_Left(_tetromino_blocks)) {
    for (i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
      _tetromino_blocks[i] = M_B_Move_Block_Left(_tetromino_blocks[i]);
    }
  }
}

void M_T_Move_Tetromino_Right(void) {
  int i;
  if (registered_flag && M_B_Can_Move_Blocks_Right(_tetromino_blocks)) {
    for (i = BLOCKS_WITHIN_A_TETROMINO - 1; i >= 0; i--) {
      _tetromino_blocks[i] = M_B_Move_Block_Right(_tetromino_blocks[i]);
    }
  }
}
