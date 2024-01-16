#ifndef R_UI_H_
#define R_UI_H_
#include "block.h"

void R_UI_Load_Textures(void);
void R_UI_Draw_Ui(void);
void R_UI_Draw_Game_Over(int score);
Texture2D *R_UI_Get_Texture_By_Block_Type(btype_t block_type);

#endif
