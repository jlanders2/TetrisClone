#ifndef M_BLOCK_H_
#define M_BLOCK_H_
#include "block.h"

void M_Create_Blocks(void);
void M_Update_Blocks(void);
block_t **M_Get_Blocks(int *length);
block_t **M_Get_Updated_Blocks(int *length);
void M_Destroy_Blocks(void);

#endif /* M_BLOCK_H_ */
