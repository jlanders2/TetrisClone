#include "r_ui.h"
#include "m_block.h"
#include "raylib.h"
#include "sc_def.h"
#include <stdlib.h>

#define UI_PADDING 16

Texture2D i_texture;
Texture2D j_texture;
Texture2D l_texture;
Texture2D o_texture;
Texture2D s_texture;
Texture2D t_texture;
Texture2D z_texture;

void R_UI_Load_Textures(void) {
  i_texture = LoadTexture("./resources/tetromino_I.png");
  j_texture = LoadTexture("./resources/tetromino_J.png");
  l_texture = LoadTexture("./resources/tetromino_L.png");
  o_texture = LoadTexture("./resources/tetromino_O.png");
  s_texture = LoadTexture("./resources/tetromino_S.png");
  t_texture = LoadTexture("./resources/tetromino_T.png");
  z_texture = LoadTexture("./resources/tetromino_Z.png");
}

void R_UI_Draw_Ui(void) {
  int up_next_box_size;
  btype_t next_block_type = M_B_Get_Next_Block_Type();
  Texture2D *next_tetromino_texture =
      R_UI_Get_Texture_By_Block_Type(next_block_type);
  BeginDrawing();
  DrawRectangle(SC_GAME_WIDTH, 0, SC_UI_WIDTH, SC_HEIGHT, SC_Gray);
  up_next_box_size = SC_UI_WIDTH - UI_PADDING * 3;
  DrawRectangle(SC_GAME_WIDTH + UI_PADDING, UI_PADDING, up_next_box_size,
                up_next_box_size, SC_Black);
  if (next_tetromino_texture) {
    DrawTexture(*next_tetromino_texture,
                (SC_GAME_WIDTH + UI_PADDING + up_next_box_size / 2) -
                    next_tetromino_texture->width / 2,
                UI_PADDING +
                    (up_next_box_size - next_tetromino_texture->height) / 2,
                SC_White);
  }
  EndDrawing();
}

void R_UI_Draw_Game_Over(int score) {
  const char *score_message = TextFormat("Score: %d", score);
  BeginDrawing();
  DrawRectangle(0, 0, SC_GAME_WIDTH, SC_HEIGHT, SC_Black);
  DrawText("Game Over", SC_GAME_WIDTH / 2 - MeasureText("Game Over", 24) / 2,
           200, 24, SC_Red);
  DrawText(score_message,
           SC_GAME_WIDTH / 2 - MeasureText(score_message, 18) / 2, 232, 18,
           SC_Red);
  /* Draw a play again button */
  EndDrawing();
}

Texture2D *R_UI_Get_Texture_By_Block_Type(btype_t block_type) {
  switch (block_type) {
  case bt_O:
    return &o_texture;
  case bt_I:
    return &i_texture;
  case bt_T:
    return &t_texture;
  case bt_J:
    return &j_texture;
  case bt_L:
    return &l_texture;
  case bt_S:
    return &s_texture;
  case bt_Z:
    return &z_texture;
  default:
    return NULL;
  }
}
