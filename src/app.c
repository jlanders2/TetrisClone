#include <stdio.h>

#include "app.h"
#include "manager.h"
#include "raylib.h"
#include "renderer.h"
#include "tetromino.h"

#define TARGET_FPS 60

int _a_tick_rate = 0;

void a_run(int tick_rate) {
  _a_initialize(tick_rate);
  _a_loop();
  _a_cleanup();
}

void _a_initialize(int tick_rate) {
  _a_initialize_raylib();
  m_initialize();
  r_initialize();
  _a_gameboard_clear();
  _a_tick_rate = tick_rate;
}

void _a_initialize_raylib(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris Clone");
  SetTargetFPS(TARGET_FPS);
}

void _a_gameboard_clear(void) {
  m_blocks_set_empty();
  int number_blocks_updated = 0;
  struct TetrominoBlock **blocks_updated =
      m_blocks_get_updated(&number_blocks_updated);
  r_render_blocks(blocks_updated, number_blocks_updated);
}

void _a_loop(void) {
  int tick_rate_counter = 0;
  while (!WindowShouldClose()) {
    if (m_tetromino_can_spawn()) {
      _a_input_process();
      if (tick_rate_counter == 0) {
        m_update();
        tick_rate_counter = _a_tick_rate;
      }
      int number_blocks_updated = 0;
      struct TetrominoBlock **blocks_updated =
          m_blocks_get_updated(&number_blocks_updated);
      r_render_blocks(blocks_updated, number_blocks_updated);
      tick_rate_counter--;
    } else {
      r_render_game_over(0);
    }
  }
}

void _a_input_process(void) {
  if (IsKeyPressed(KEY_SPACE)) {
    m_request_falling_tetromino_rotate();
  }
  if (IsKeyDown(KEY_A)) {
    m_request_falling_tetromino_move_left();
  } else if (IsKeyDown(KEY_D)) {
    m_request_falling_tetromino_move_right();
  }
}

void _a_cleanup() {
  m_deactivate();
  CloseWindow();
}
