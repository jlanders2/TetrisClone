#ifndef POINT_H_
#define POINT_H_
#include "point.h"

typedef struct point_t
{
    unsigned int x;
    unsigned int y;
} point_t;

typedef struct point_offset_t
{
    int xOffset;
    int yOffset;
} point_offset_t;

#endif /* POINT_H_ */
