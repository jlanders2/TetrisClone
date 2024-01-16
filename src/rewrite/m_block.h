#ifndef M_BLOCK_H_
#define M_BLOCK_H_
#include "block.h"

void M_B_Create_Blocks(void);
void M_B_Update_Blocks(void);
block_t **M_B_Get_Blocks(int *length);
block_t **M_B_Get_Updated_Blocks(int *length);
void M_B_Destroy_Blocks(void);
void M_B_Reset_Blocks_Updated(void);
void M_B_Spawn_Blocks(void);
void M_B_Set_Next_Block_Type(void);
btype_t M_B_Get_Random_Block_Type(void);
int M_B_Try_Spawn_Blocks_With_Offset(point_offset_t *offsets);
block_t *M_B_Get_Block_At_Point(point_t point);
void M_B_Set_Block_Type(block_t *block, btype_t type);
void M_B_Register_Updated_block(block_t *block);
/* TODO: I don't need ** for these kind of functions */
int M_B_Can_Move_Blocks_Left(block_t **blocks);
int M_B_Can_Move_Block_Left(block_t block);
block_t *M_B_Move_Block_Left(block_t *block);
/* TODO: I don't need ** for these kind of functions */
int M_B_Can_Move_Blocks_Right(block_t **blocks);
int M_B_Can_Move_Block_Right(block_t block);
block_t *M_B_Move_Block_Right(block_t *block);
/* TODO: I don't need ** for these kind of functions */
int M_B_Can_Move_Blocks_Down(block_t **blocks);
int M_B_Can_Move_Block_Down(block_t block);
block_t *M_B_Move_Block_Down(block_t *block);
block_t *M_B_Get_Block_At_Offset(block_t *block, point_offset_t offset);
void M_B_On_Block_Spawn(void (*callback)(block_t **blocks));
int M_B_Point_Intersects_Static_Block(point_t point, int id);
int M_B_Generate_Block_Id(void);
int M_B_Can_Spawn_Blocks(void);
block_t *M_B_Rotate_Block_Around_Point(block_t *block_to_rotate,
                                       point_t rotation_point);

#endif /* M_BLOCK_H_ */
