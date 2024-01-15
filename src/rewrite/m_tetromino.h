#ifndef M_TETROMINO_H_
#define M_TETROMINO_H_
#include "block.h"

void M_T_Update_Tetromino(void);
void M_T_Apply_To_Tetromino_Blocks(block_t *(*callback)(block_t *block));
void M_T_Tetromino_Fall(void);
void M_T_Register_Falling_Blocks(block_t **blocks);
void M_T_Move_Tetromino_Left(void);
void M_T_Move_Tetromino_Right(void);

#endif /* M_TETROMINO_H_ */
