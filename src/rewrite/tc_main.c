#include "m_block.h"
#include "m_tetromino.h"
#include "r_block.h"
#include "r_ui.h"
#include "tc_input.h"
#include "tc_settings.h"
#include "tc_window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void TC_Start(void);
void TC_Initialize(void);
void TC_Game_Loop(void);
void TC_Stop(void);

int main(int c, char *v[])
{
    /* Move somwhere else to parse options */
    /* =================================== */
    if (c > 1)
    {
        if (strcmp(v[1], "--speed") == 0)
        {
            TC_Set_Game_Speed(atoi(v[2]));
        }
    }
    /* =================================== */

    TC_Start();
    TC_Stop();
}

void TC_Start(void)
{
    TC_Initialize();
    TC_Game_Loop();
}

void TC_Initialize(void)
{
    int n_blocks_created = 0;
    block_t **blocks_created = NULL;
    TC_Create_Window();
    M_Create_Blocks();
    R_Load_Textures();
    blocks_created = M_Get_Blocks(&n_blocks_created);
    R_Draw_Blocks(blocks_created, n_blocks_created);
}

void TC_Game_Loop(void)
{
    int tick_rate, n_updated_blocks = 0;
    block_t **updated_blocks = NULL;
    while (!TC_Close_Window())
    {
        R_Draw_Ui();
        if (tick_rate == 0)
        {
            TC_Process_Input();
            M_Update_Tetromino();
            M_Update_Blocks();
            M_Get_Updated_Blocks(&n_updated_blocks);
            R_Draw_Blocks(updated_blocks, n_updated_blocks);
            tick_rate = game_speed_setting;
        }
        else
        {
            tick_rate--;
        }
    }
    R_Draw_Game_Over(0);
    getchar(); /* putting this here just to stop prog from ending atm */
}

void TC_Stop(void)
{
    M_Destroy_Blocks();
    TC_Close_Window();
}
