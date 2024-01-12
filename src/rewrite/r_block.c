#include "r_block.h"
#include "sc_def.h"

#define RES_PNG_FILE(name) "resources/" #name ".png"
#define TEXTURE_SIZE 32

Texture2D yellow_texture;
Texture2D blue_texture;
Texture2D light_blue_texture;
Texture2D red_texture;
Texture2D purple_texture;
Texture2D green_texture;
Texture2D orange_texture;
Texture2D empty_texture;

void R_Load_Textures(void)
{
    yellow_texture = LoadTexture(RES_PNG_FILE(yellow));
    blue_texture = LoadTexture(RES_PNG_FILE(blue));
    light_blue_texture = LoadTexture(RES_PNG_FILE(light - blue));
    red_texture = LoadTexture(RES_PNG_FILE(red));
    purple_texture = LoadTexture(RES_PNG_FILE(purple));
    green_texture = LoadTexture(RES_PNG_FILE(green));
    orange_texture = LoadTexture(RES_PNG_FILE(orange));
    empty_texture = LoadTexture(RES_PNG_FILE(black));
}

void R_Draw_Blocks(block_t **blocks, int block_count)
{
    BeginDrawing();
    while (block_count--)
    {
        R_Draw_Block(*(*blocks));
        blocks++;
    }
    /* implement */
    EndDrawing();
}

void R_Draw_Block(block_t block)
{
    Texture2D *texture = __R_Get_Texture_By_Block_Type(block.type);
    DrawTexture(*texture, block.point.x * TEXTURE_SIZE, block.point.y * TEXTURE_SIZE, SC_White);
}

Texture2D *__R_Get_Texture_By_Block_Type(btype_t block_type)
{
    switch (block_type)
    {
    case bt_O:
        return &yellow_texture;
    case bt_I:
        return &light_blue_texture;
    case bt_T:
        return &purple_texture;
    case bt_J:
        return &blue_texture;
    case bt_L:
        return &orange_texture;
    case bt_S:
        return &green_texture;
    case bt_Z:
        return &red_texture;
    default:
        return &empty_texture;
    }
}