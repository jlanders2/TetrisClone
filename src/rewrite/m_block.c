#include "m_block.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BLOCK_ARRAY_COLUMNS 10
#define BLOCK_ARRAY_ROWS 20
#define BLOCK_ARRAY_LENGTH 200

block_t *_blocks[BLOCK_ARRAY_LENGTH];
block_t *_updated_blocks[BLOCK_ARRAY_LENGTH];
block_t *_recently_spawned_blocks[BLOCKS_WITHIN_A_TETROMINO];
int _updated_blocks_current_length = 0;
int _spawn_tetromino_flag = 1;
int _can_spawn_tetromino_flag = 1;
btype_t _next_block_type = bt_Empty;
const int _spawn_x = BLOCK_ARRAY_COLUMNS / 2 - 1;
const point_offset_t _shift_down_offset = {0, 1};
const point_offset_t _shift_left_offset = {-1, 0};
const point_offset_t _shift_right_offset = {1, 0};
void (*on_block_spawn)(block_t *blocks[BLOCKS_WITHIN_A_TETROMINO]);

void M_B_Create_Blocks(void) {
  int i;
  block_t *block = NULL;

  for (i = 0; i < BLOCK_ARRAY_LENGTH; i++) {
    block = malloc(sizeof(block_t));
    block->id = -1;
    block->point.x = i % BLOCK_ARRAY_COLUMNS;
    block->point.y = i / BLOCK_ARRAY_COLUMNS;
    block->type = bt_Empty;
    _blocks[i] = block;
  }
}

void M_B_Update_Blocks(void) {
  M_B_Reset_Blocks_Updated();
  if (_spawn_tetromino_flag) {
    M_B_Spawn_Blocks();
    M_B_Set_Next_Block_Type();
    _spawn_tetromino_flag = 0;
  }
}

block_t **M_B_Get_Blocks(int *length) {
  *length = BLOCK_ARRAY_LENGTH;
  return _blocks;
}

block_t **M_B_Get_Updated_Blocks(int *length) {
  *length = _updated_blocks_current_length;
  return _updated_blocks;
}

void M_B_Destroy_Blocks(void) {
  int i;

  for (i = 0; i < BLOCK_ARRAY_LENGTH; i++) {
    free(_blocks[i]);
  }
}

void M_B_Reset_Blocks_Updated(void) { _updated_blocks_current_length = 0; }

void M_B_On_Block_Spawn(void (*callback)(block_t **blocks)) {
  on_block_spawn = callback;
}

void M_B_Spawn_Blocks(void) {
  struct point_offset_t offsets[BLOCKS_WITHIN_A_TETROMINO];

  if (_next_block_type == bt_Empty) {
    M_B_Set_Next_Block_Type();
  }
  switch (_next_block_type) {
  case bt_I:
    offsets[0].x_offset = 0;
    offsets[0].y_offset = 0;

    offsets[1].x_offset = 0;
    offsets[1].y_offset = 1;

    offsets[2].x_offset = 0;
    offsets[2].y_offset = 2;

    offsets[3].x_offset = 0;
    offsets[3].y_offset = 3;
    break;
  case bt_J:
    offsets[0].x_offset = 2;
    offsets[0].y_offset = 0;

    offsets[1].x_offset = 0;
    offsets[1].y_offset = 1;

    offsets[2].x_offset = 1;
    offsets[2].y_offset = 1;

    offsets[3].x_offset = 2;
    offsets[3].y_offset = 1;
    break;
  case bt_L:
    offsets[0].x_offset = 0;
    offsets[0].y_offset = 0;

    offsets[1].x_offset = 0;
    offsets[1].y_offset = 1;

    offsets[2].x_offset = 1;
    offsets[2].y_offset = 1;

    offsets[3].x_offset = 2;
    offsets[3].y_offset = 1;
    break;
  case bt_O:
    offsets[0].x_offset = 0;
    offsets[0].y_offset = 0;

    offsets[1].x_offset = 1;
    offsets[1].y_offset = 0;

    offsets[2].x_offset = 0;
    offsets[2].y_offset = 1;

    offsets[3].x_offset = 1;
    offsets[3].y_offset = 1;
    break;
  case bt_S:
    offsets[0].x_offset = 1;
    offsets[0].y_offset = 0;

    offsets[1].x_offset = 2;
    offsets[1].y_offset = 0;

    offsets[2].x_offset = 0;
    offsets[2].y_offset = 1;

    offsets[3].x_offset = 1;
    offsets[3].y_offset = 1;
    break;
  case bt_T:
    offsets[0].x_offset = 1;
    offsets[0].y_offset = 0;

    offsets[1].x_offset = 0;
    offsets[1].y_offset = 1;

    offsets[2].x_offset = 1;
    offsets[2].y_offset = 1;

    offsets[3].x_offset = 2;
    offsets[3].y_offset = 1;
    break;
  case bt_Z:
    offsets[0].x_offset = 0;
    offsets[0].y_offset = 0;

    offsets[1].x_offset = 1;
    offsets[1].y_offset = 0;

    offsets[2].x_offset = 1;
    offsets[2].y_offset = 1;

    offsets[3].x_offset = 2;
    offsets[3].y_offset = 1;
    break;
  default:
    break;
  }
  if (M_B_Try_Spawn_Blocks_With_Offset(offsets)) {
    on_block_spawn(_recently_spawned_blocks);
  }
}

void M_B_Set_Next_Block_Type(void) {
  _next_block_type = M_B_Get_Random_Block_Type();
}

btype_t M_B_Get_Random_Block_Type(void) {
  srand(time(NULL));
  return (btype_t)(rand() % 7) + 1;
}

int M_B_Try_Spawn_Blocks_With_Offset(point_offset_t *offsets) {
  int i = 0, unique_id = M_B_Generate_Block_Id();
  point_t spawn_point;
  block_t *spawn_block;

  while (i < BLOCKS_WITHIN_A_TETROMINO) {
    spawn_point.x = _spawn_x + offsets->x_offset;
    spawn_point.y = offsets->y_offset;
    spawn_block = M_B_Get_Block_At_Point(spawn_point);
    if (spawn_block->type != bt_Empty) {
      _can_spawn_tetromino_flag = 0;
      return 0;
    }
    spawn_block->id = unique_id;
    M_B_Set_Block_Type(spawn_block, _next_block_type);
    _recently_spawned_blocks[i] = spawn_block;
    i++;
    offsets++;
  }
  return 1;
}

block_t *M_B_Get_Block_At_Point(point_t point) {
  return _blocks[(point.x % BLOCK_ARRAY_COLUMNS) +
                 (point.y * BLOCK_ARRAY_COLUMNS)];
}

void M_B_Set_Block_Type(block_t *block, btype_t type) {
  block->type = type;
  M_B_Register_Updated_block(block);
}

void M_B_Register_Updated_block(block_t *block) {
  if (_updated_blocks_current_length < BLOCK_ARRAY_LENGTH) {
    _updated_blocks[_updated_blocks_current_length] = block;
    _updated_blocks_current_length++;
  }
}

int M_B_Can_Move_Blocks_Left(block_t **blocks) {
  int i, result = 1;

  for (i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
    if (!M_B_Can_Move_Block_Left(*(blocks[i]))) {
      result = 0;
      break;
    }
  }
  return result;
}

int M_B_Can_Move_Block_Left(block_t block) {
  point_t left_adj_block_point;

  left_adj_block_point.x = block.point.x - 1;
  left_adj_block_point.y = block.point.y;
  if (left_adj_block_point.x < 0) {
    return 0;
  } else if (M_B_Point_Intersects_Static_Block(left_adj_block_point,
                                               block.id)) {
    return 0;
  } else {
    return 1;
  }
}

block_t *M_B_Move_Block_Left(block_t *block) {
  /* Kinda dupe from right and down, might consolidate */
  block_t *updated_block = M_B_Get_Block_At_Offset(block, _shift_left_offset);

  updated_block->id = block->id;
  M_B_Set_Block_Type(updated_block, block->type);
  block->id = -1;
  M_B_Set_Block_Type(block, bt_Empty);
  return updated_block;
}

int M_B_Can_Move_Blocks_Right(block_t **blocks) {
  int i, result = 1;

  for (i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
    if (!M_B_Can_Move_Block_Right(*(blocks[i]))) {
      result = 0;
      break;
    }
  }
  return result;
}

int M_B_Can_Move_Block_Right(block_t block) {
  point_t right_adj_block_point;

  right_adj_block_point.x = block.point.x + 1;
  right_adj_block_point.y = block.point.y;
  if (right_adj_block_point.x >= BLOCK_ARRAY_COLUMNS) {
    return 0;
  } else if (M_B_Point_Intersects_Static_Block(right_adj_block_point,
                                               block.id)) {

    return 0;
  } else {
    return 1;
  }
}

block_t *M_B_Move_Block_Right(block_t *block) {
  block_t *updated_block = M_B_Get_Block_At_Offset(block, _shift_right_offset);

  updated_block->id = block->id;
  M_B_Set_Block_Type(updated_block, block->type);
  block->id = -1;
  M_B_Set_Block_Type(block, bt_Empty);
  return updated_block;
}

int M_B_Can_Move_Blocks_Down(block_t **blocks) {
  int i, result = 1;

  for (i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
    if (!M_B_Can_Move_Block_Down(*(blocks[i]))) {
      result = 0;
      break;
    }
  }
  if (!result) {
    _spawn_tetromino_flag = 1;
  }
  return result;
}

int M_B_Can_Move_Block_Down(block_t block) {
  point_t bottom_adj_block_point;

  bottom_adj_block_point.x = block.point.x;
  bottom_adj_block_point.y = block.point.y + 1;
  if (bottom_adj_block_point.y >= BLOCK_ARRAY_ROWS) {
    return 0;
  } else if (M_B_Point_Intersects_Static_Block(bottom_adj_block_point,
                                               block.id)) {
    return 0;
  } else {
    return 1;
  }
}

block_t *M_B_Move_Block_Down(block_t *block) {
  block_t *updated_block = M_B_Get_Block_At_Offset(block, _shift_down_offset);

  updated_block->id = block->id;
  M_B_Set_Block_Type(updated_block, block->type);
  block->id = -1;
  M_B_Set_Block_Type(block, bt_Empty);
  return updated_block;
}

block_t *M_B_Get_Block_At_Offset(block_t *block, point_offset_t offset) {
  point_t updated_point;

  updated_point.x = block->point.x + offset.x_offset;
  updated_point.y = block->point.y + offset.y_offset;
  return M_B_Get_Block_At_Point(updated_point);
}

int M_B_Point_Intersects_Static_Block(point_t point, int id) {
  int result = 0;
  block_t *block_at_point = M_B_Get_Block_At_Point(point);

  if (block_at_point->type != bt_Empty && block_at_point->id != id) {
    result = 1;
  }
  return result;
}

int M_B_Generate_Block_Id(void) {
  /* The odds of this not being unique are high enough, surely
     SURELY
  */
  srand(time(NULL));
  return rand();
}

int M_B_Can_Spawn_Blocks(void) { return _can_spawn_tetromino_flag; }

/* TODO: There is an issue with S and T Type Rotations */
block_t *M_B_Rotate_Block_Around_Point(block_t *block_to_rotate,
                                       point_t rotation_point) {
  point_offset_t offset, transposed_offset;
  block_t *updated_block,
      *origin_block = M_B_Get_Block_At_Point(rotation_point);
  btype_t current_block_type = block_to_rotate->type;
  int current_block_id = block_to_rotate->id;

  offset.x_offset = block_to_rotate->point.x - rotation_point.x;
  offset.y_offset = block_to_rotate->point.y - rotation_point.y;
  if (offset.x_offset > 0 || offset.x_offset < 0) {
    transposed_offset.x_offset = offset.y_offset;
    transposed_offset.y_offset = offset.x_offset * -1;
  } else {
    transposed_offset.x_offset = offset.y_offset;
    transposed_offset.y_offset = offset.x_offset;
  }
  updated_block = M_B_Get_Block_At_Offset(origin_block, transposed_offset);
  block_to_rotate->id = -1;
  M_B_Set_Block_Type(block_to_rotate, bt_Empty);
  updated_block->id = current_block_id;
  M_B_Set_Block_Type(updated_block, current_block_type);
  return updated_block;
}
