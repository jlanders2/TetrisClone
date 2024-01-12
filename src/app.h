#ifndef TETRIS_CLONE_APP_H_
#define TETRIS_CLONE_APP_H_

void a_run(int tick_rate);

void _a_initialize(int tick_rate);
void _a_initialize_raylib(void);
void _a_gameboard_clear(void);
void _a_loop(void);
void _a_input_process(void);
void _a_cleanup(void);

#endif // TETRIS_CLONE_APP_H_
