#include "m_tetromino.h"
#include "m_block.h"
#include <limits.h>
#include <stdlib.h>

/* Blocks are gauranteed to be in order top left to bottom right */
block_t *_tetromino_blocks[BLOCKS_WITHIN_A_TETROMINO];
point_t origin_point;
int registered_flag = 0;

void M_T_Update_Tetromino(void) { M_T_Tetromino_Fall(); }

void M_T_Tetromino_Fall(void) {
  int i;
  if (registered_flag && M_B_Can_Move_Blocks_Down(_tetromino_blocks)) {
    for (i = BLOCKS_WITHIN_A_TETROMINO - 1; i >= 0; i--) {
      _tetromino_blocks[i] = M_B_Move_Block_Down(_tetromino_blocks[i]);
    }
    origin_point.y++;
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
  origin_point = _tetromino_blocks[0]->point;
}

void M_T_Move_Tetromino_Left(void) {
  int i;
  if (registered_flag && M_B_Can_Move_Blocks_Left(_tetromino_blocks)) {
    for (i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
      _tetromino_blocks[i] = M_B_Move_Block_Left(_tetromino_blocks[i]);
    }
    origin_point.x--;
  }
}

void M_T_Move_Tetromino_Right(void) {
  int i;
  if (registered_flag && M_B_Can_Move_Blocks_Right(_tetromino_blocks)) {
    for (i = BLOCKS_WITHIN_A_TETROMINO - 1; i >= 0; i--) {
      _tetromino_blocks[i] = M_B_Move_Block_Right(_tetromino_blocks[i]);
    }
    origin_point.x++;
  }
}

int M_T_Block_Compare_Function(const void *a, const void *b) {
  const block_t *block_a = a, *block_b = b;
  return block_a < block_b;
}

void M_T_Rotate_Tetromino(void) {
  int i;
  printf("\n\n");
  for (i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
    printf("%d: %p\n", i, (_tetromino_blocks[i]));
  }
  if (registered_flag) {
    for (i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
      _tetromino_blocks[i] =
          M_B_Rotate_Block_Around_Point(_tetromino_blocks[i], origin_point);
    }
    qsort(_tetromino_blocks, BLOCKS_WITHIN_A_TETROMINO, sizeof(block_t *),
          M_T_Block_Compare_Function);
  }
  printf("\n\n");
}
