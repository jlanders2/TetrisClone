#include "m_block.h"
#include "block.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BLOCK_ARRAY_COLUMNS 10
#define BLOCK_ARRAY_ROWS 20
#define BLOCK_ARRAY_LENGTH 200
#define MAX_FULL_ROWS 4

#define DEBUG

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
int _top_row = 0;

/* TODO: put internal function declarations here */
void M_B_Get_Top_Row(void);
int M_B_Get_Full_Rows(int **indices, int *length);
void M_B_Drop_Rows(int *indices, int indices_length);
void M_B_Shift_Rows_Down(int shift_to_index, int row_count);
void M_B_Shift_Row_Down(int index);

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
  int full_indices_length = 0;
  int *full_indices = NULL;
  M_B_Reset_Blocks_Updated();
  M_B_Get_Top_Row();
  if (_spawn_tetromino_flag &&
      _next_block_type !=
          bt_Empty && /* Probably better way to flag this, tired*/
      (M_B_Get_Full_Rows(&full_indices, &full_indices_length)) > 0) {
    /* update score based on the number of full rows */
    M_B_Drop_Rows(full_indices, full_indices_length);
  }
  if (_spawn_tetromino_flag) {
    M_B_Spawn_Blocks();
    M_B_Set_Next_Block_Type();
    _spawn_tetromino_flag = 0;
  }
  if (full_indices != NULL) {
    free(full_indices);
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
    offsets[0].x_offset = 0;
    offsets[0].y_offset = 0;

    offsets[1].x_offset = 0;
    offsets[1].y_offset = 1;

    offsets[2].x_offset = 1;
    offsets[2].y_offset = 1;

    offsets[3].x_offset = 2;
    offsets[3].y_offset = 1;
    break;
  case bt_L:
    offsets[0].x_offset = 2;
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

void M_B_Set_Next_Block_Type(void) { _next_block_type = bt_I; }

btype_t M_B_Get_Next_Block_Type(void) { return _next_block_type; }

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

void M_B_Get_Top_Row(void) {
  int i, j, counter;
  point_t point;
  block_t *block;
  for (i = BLOCK_ARRAY_ROWS - 1; i >= 0; i--) {
    counter = 0;
    for (j = 0; j < BLOCK_ARRAY_COLUMNS; j++) {
      point.x = j;
      point.y = i;
      block = M_B_Get_Block_At_Point(point);
      if (block->type == bt_Empty)
        counter++;
    }
    if (counter == BLOCK_ARRAY_COLUMNS) {
      _top_row = i;
      break;
    }
  }
}

/* This function is WILD LOL */
int M_B_Get_Full_Rows(int **full_indices, int *length) {
  int i, j, k, counter = 0;
  int *temp;
  point_t point;
  block_t *block;
  for (i = BLOCK_ARRAY_ROWS; i >= _top_row; i--) {
    counter = 0;
    for (j = 0; j < BLOCK_ARRAY_COLUMNS; j++) {
      point.x = j;
      point.y = i;
      block = M_B_Get_Block_At_Point(point);
      if (block->type != bt_Empty)
        counter++;
    }
    if (counter == BLOCK_ARRAY_COLUMNS) {
      (*length)++;
      if (*full_indices == NULL) {
        *full_indices = malloc(sizeof(int));
        (*full_indices)[0] = i;
      } else {
        temp = malloc(*length * sizeof(int));
        for (k = 0; k < *length; k++) {
          if (k <= *length - 2) {
            temp[k] = (*full_indices)[k];
          } else {
            temp[k] = i;
          }
        }
        free(*full_indices);
        *full_indices = temp;
      }
    }
  }
  printf("Full row count = %d\n", *length);

  return *length;
}

int sort_desc(const void *a, const void *b) {
  int arg1 = *(const int *)a;
  int arg2 = *(const int *)b;
  if (arg1 > arg2)
    return -1;
  if (arg1 < arg2)
    return 1;
  return 0;
}

void M_B_Drop_Rows(int *indices, int indices_length) {
  qsort(indices, indices_length, sizeof(int), sort_desc);

  M_B_Shift_Rows_Down(indices[0], indices_length);
}

void M_B_Shift_Rows_Down(int shift_to_index, int row_count) {
  int i, j;

  for (i = 0; i < row_count; i++) {
    for (j = shift_to_index - 1; j >= _top_row; j--) {
      M_B_Shift_Row_Down(j);
    }
    _top_row++;
  }
}

void M_B_Shift_Row_Down(int index) {
  int i;
  block_t *block;
  point_t point;
  for (i = 0; i < BLOCK_ARRAY_COLUMNS; i++) {
    point.x = i;
    point.y = index;
    block = M_B_Get_Block_At_Point(point);
    M_B_Move_Block_Down(block);
  }
}
