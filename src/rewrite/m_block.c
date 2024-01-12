#include "m_block.h"
#include <stdlib.h>

#define BLOCK_ARRAY_COLUMNS 10
#define BLOCK_ARRAY_ROWS 20
#define BLOCK_ARRAY_LENGTH 200

block_t *_blocks[BLOCK_ARRAY_LENGTH];
block_t *_updated_blocks[BLOCK_ARRAY_LENGTH];
int updated_blocks_current_length = 0;

void M_Create_Blocks(void)
{
    int i;
    for (i = 0; i < BLOCK_ARRAY_LENGTH; i++)
    {
        block_t *block = malloc(sizeof(block_t));
        block->point.x = i % BLOCK_ARRAY_COLUMNS;
        block->point.y = i / BLOCK_ARRAY_COLUMNS;
        block->type = bt_Empty;

        _blocks[i] = block;
    }
}

void M_Update_Blocks(void)
{
    /* implement */
}

block_t **M_Get_Blocks(int *length)
{
    *length = BLOCK_ARRAY_LENGTH;
    return _blocks;
}

block_t **M_Get_Updated_Blocks(int *length)
{
    *length = updated_blocks_current_length;
    return _updated_blocks;
}

void M_Destroy_Blocks(void)
{
    int i;
    for (i = 0; i < BLOCK_ARRAY_LENGTH; i++)
    {
        free(_blocks[i]);
    }
}
