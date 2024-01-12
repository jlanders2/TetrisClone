#include "tc_window.h"
#include "raylib.h"
#include "sc_def.h"
#include "tc_settings.h"

void TC_Create_Window(void)
{
    InitWindow(SC_WIDTH, SC_HEIGHT, "Tetris Clone");
    SetTargetFPS(target_fps_setting);
}

int TC_Close_Window(void)
{
    return WindowShouldClose();
}
