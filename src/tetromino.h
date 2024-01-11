#ifndef TETRIS_CLONE_TETROMINO_H_
#define TETRIS_CLONE_TETROMINO_H_

enum TetrominoType {
  I_TYPE,
  J_TYPE,
  L_TYPE,
  O_TYPE,
  S_TYPE,
  T_TYPE,
  Z_TYPE,
  EMPTY 
};

struct TetrominoBlock {
  unsigned char x;
  unsigned char y;
  enum TetrominoType type;
};

#endif
