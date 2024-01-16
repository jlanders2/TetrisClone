#ifndef BLOCK_H_
#define BLOCK_H_
#include "point.h"

/* Not sure where this belongs tbh */
#define BLOCKS_WITHIN_A_TETROMINO 4

typedef enum btype_t {
  bt_Empty,
  bt_I,
  bt_J,
  bt_L,
  bt_O,
  bt_S,
  bt_T,
  bt_Z
} btype_t;

typedef struct block_t {
  int id;
  point_t point;
  btype_t type;
} block_t;

#endif /* BLOCK_H_ */
