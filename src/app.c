#include <stdio.h>

#include "raylib.h"
#include "manager.h"
#include "renderer.h"
#include "tetromino.h"
#include "app.h"

#define TARGET_FPS 60

void a_run(void) {
    _a_initialize();
    _a_loop();
    _a_cleanup();
}

void _a_initialize(void) {
    _a_initialize_raylib();
    m_initialize();
    r_initialize();
    _a_gameboard_clear();
}

void _a_initialize_raylib(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris Clone");
    SetTargetFPS(TARGET_FPS);
}

void _a_gameboard_clear(void) {
    m_blocks_set_empty();
    int number_blocks_updated = 0;
    struct TetrominoBlock** blocks_updated = m_blocks_get_updated(&number_blocks_updated);
    r_render_blocks(blocks_updated, number_blocks_updated);
}

void _a_loop(void) {
    int tick_rate = 0; // Guaranteed a better way to do this XD
    while(!WindowShouldClose()) {
        while(m_tetromino_can_spawn()) {
            _a_input_process();
            if (tick_rate == 0) {
                m_update();
                tick_rate = 50;
            }
            int number_blocks_updated = 0;
            struct TetrominoBlock** blocks_updated = GetUpdatedBlocks(&number_blocks_updated);
            r_render_blocks(blocks_updated, number_blocks_updated);
            tick_rate--;
        }
        r_render_game_over(0);
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
