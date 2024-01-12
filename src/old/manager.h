#ifndef TETRIS_CLONE_MANAGER_H_
#define TETRIS_CLONE_MANAGER_H_

#include <stdbool.h>

#include "tetromino.h"
void m_initialize(void);
void m_update(void);
void m_blocks_set_empty(void);
bool m_tetromino_can_spawn(void); // Bad name + Doesn't follow naming
void m_request_falling_tetromino_rotate(void);
void m_request_falling_tetromino_move_left(void);
void m_request_falling_tetromino_move_right(void);
struct TetrominoBlock **m_blocks_get_updated(int *lengthOfBlocksUpdated);
void m_deactivate(void);

#endif
