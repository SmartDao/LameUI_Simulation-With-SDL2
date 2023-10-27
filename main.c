#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "sdl_driver.h"
#include "lame_ui.h"








void my_render_complete_handler()
{

	g_disp_buffer_counter = 0; //reset the counter
}





int mainLoop(void)
{


	// [IMPORTANT:] do it at the begining. Mandatory!
	uint8_t memblk[20000];
	lui_init(memblk, sizeof(memblk));


	lame_register_dispaly_driver();
	lame_register_input();


	//demo_create();
	//widget_test();
	//simple_example();
	//big_example();
	music_demo();
    while (1)
    {

        SDL_Delay(10);   /*Sleep for 5 millisecond*/
        lui_update();
        SDL_RenderPresent(gRenderer);// Ë¢ÐÂ´°¿Ú
		g_disp_buffer_counter = 0; //reset the counter
        if(get_SDL_Event()==-1)
        {
            return -1;
        }
    }
    return 0;
}

int main(int argc,char* args[])
{
    if (!init_SDL2()) return -1;
    mainLoop();

    close_SDL2();
    return 0;
}
