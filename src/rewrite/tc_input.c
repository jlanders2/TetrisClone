#include "tc_input.h"
#include "m_tetromino.h"
#include "raylib.h"

/* Was having issues with raylib input, wasn't having these
   issues before but I figured out that I can just cast to
   a char to get the result I want :)

   should look into what's going on though to understand better
*/

void TC_Process_Input_Per_Tick(void) {
  if ((char)IsKeyDown(KEY_A)) {
    M_T_Move_Tetromino_Left();
  } else if ((char)IsKeyDown(KEY_D)) {
    M_T_Move_Tetromino_Right();
  }
}

void TC_Process_Input_Per_Frame(void) {
  if ((char)IsKeyPressed(KEY_SPACE)) {
    M_T_Rotate_Tetromino();
  }
}
