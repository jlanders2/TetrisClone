#ifndef TETRIS_CLONE_TETROMINO_H_
#define TETRIS_CLONE_TETROMINO_H_

enum TetrominoType {
  TT_I,
  TT_J,
  TT_L,
  TT_O,
  TT_S,
  TT_T,
  TT_Z,
  TT_EMPTY 
};

struct TetrominoBlock {
  unsigned int x;
  unsigned int y;
  enum TetrominoType type;
};

#endif
