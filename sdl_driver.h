#ifndef __SDL_DRIVER_H
#define	__SDL_DRIVER_H

#include  "SDL2/include/SDL2/SDL.h"
#include  "SDL2/include/SDL2/SDL_keyboard.h"
#include  "SDL2/include/SDL2/SDL_keycode.h"
#include "lame_ui.h"
#include "lcd_config.h"
#include  <stdio.h>


extern SDL_Window* gWindow;
extern SDL_Surface* gScreenSurface ;
extern SDL_Renderer* gRenderer ;

extern uint32_t g_disp_buffer_counter;
extern lui_touch_input_data_t g_input;
int init_SDL2(void);
void close_SDL2(void);
void SDL_DrawPoint ( int x, int y, int c );
int get_SDL_Event(void);
void draw_disp_buff_cb(uint16_t* disp_buff, lui_area_t* area);
void my_input_read_sdl (lui_touch_input_data_t *input);

void lame_register_dispaly_driver(void);
void lame_register_input(void);
#endif

