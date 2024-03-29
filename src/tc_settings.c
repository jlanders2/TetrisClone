#include "tc_settings.h"

#define DEFAULT_GAME_SPEED 50

int game_speed_setting = 50;
int target_fps_setting = 60;

struct TC_Setting *tc_settings;

void TC_Set_Game_Speed(int speed) { game_speed_setting = speed; }

void TC_Set_Target_Fps(int fps) { target_fps_setting = fps; }

void TC_Reset_Game_Speed(void) { game_speed_setting = DEFAULT_GAME_SPEED; }
