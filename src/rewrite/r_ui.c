#include "r_ui.h"
#include "raylib.h"
#include "sc_def.h"

#define UI_PADDING 16

void R_Draw_Ui(void) {
  int up_next_box_size;
  BeginDrawing();
  DrawRectangle(SC_GAME_WIDTH, 0, SC_UI_WIDTH, SC_HEIGHT, SC_Gray);
  up_next_box_size = SC_UI_WIDTH - UI_PADDING * 3;
  DrawRectangle(SC_GAME_WIDTH + UI_PADDING, 0 + UI_PADDING, up_next_box_size,
                up_next_box_size, SC_Black);
  EndDrawing();
}

void R_Draw_Game_Over(int score) {
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
