#ifndef BLOCK_H_
#define BLOCK_H_
#include "point.h"

/* still unsure how i want to name structs and enums */

typedef enum btype_t
{
    bt_I,
    bt_J,
    bt_L,
    bt_O,
    bt_S,
    bt_T,
    bt_Z,
    bt_Empty
} btype_t;

typedef struct block_t
{
    point_t point;
    btype_t type;
} block_t;

#endif /* BLOCK_H_ */
