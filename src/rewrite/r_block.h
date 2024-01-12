#ifndef R_BLOCK_H_
#define R_BLOCK_H_
#include "block.h"
#include "raylib.h"

void R_Load_Textures(void);
void R_Draw_Blocks(block_t **blocks, int block_count);
void R_Draw_Block(block_t block);
Texture2D *__R_Get_Texture_By_Block_Type(btype_t block_type);

#endif /* R_BLOCK_H_ */
