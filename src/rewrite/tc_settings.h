#ifndef TC_SETTINGS_H_
#define TC_SETTINGS_H_

/* these settings could probably be more robust, probably doesn't matter until i
 * need more*/
/* Maybe have a load defaults, then optionally update? */

void TC_Set_Game_Speed(int speed);
void TC_Set_Target_Fps(int fps);

extern int game_speed_setting;
extern int target_fps_setting;

#endif /* TC_SETTINGS_H_ */
