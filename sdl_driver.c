#include "sdl_driver.h"
#include  "lame_ui.h"

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* gRenderer = NULL;

const int SCREEN_WIDTH = LCD_SCREEN_WIDTH;
const int SCREEN_HEIGHT = LCD_SCREEN_HEIGHT;
g_sidp_buffer_max_size = LCD_SCREEN_WIDTH * LCD_SCREEN_HEIGHT;
uint32_t g_disp_buffer_counter = 0;

#define DISP_BUFF_PX_CNT (LCD_SCREEN_WIDTH * 10)
uint16_t disp_buffer[DISP_BUFF_PX_CNT];

lui_touch_input_data_t g_input =
{
	.is_pressed = 0,
	.x = -1,
	.y = -1
};



int init_SDL2(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL can not initialized!SDL Error:%s\n", SDL_GetError());
        return 0;
    }
    //放大像素
    gWindow = SDL_CreateWindow("LameUI", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
        printf("SDL can't create window!SDL Error:%s\n", SDL_GetError());
        return -1;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL)
    {
        return 0;
    }
    //----------------SDL2---------------------------------//
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xff);     // bg
    printf("open WindowShow \n");
    return 1;
}


void close_SDL2(void)
{

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}


void SDL_DrawPoint ( int x, int y, int c )
{
    Uint8 r,g,b;
    /* Convert RGB565 to RGB888 */
    r = (c>>11)&0x1F;
    r<<=3;
    g = (c>>5)&0x3F;
    g<<=2;
    b = (c)&0x1F;
    b<<=3;
    SDL_SetRenderDrawColor(gRenderer,(Uint8)r, (Uint8)g, (Uint8)b, 0xff);
#if 0
    SDL_RenderDrawPoint(gRenderer, 2*x,2*y);        //放大像素
    SDL_RenderDrawPoint(gRenderer, 2*x+1,2*y);
    SDL_RenderDrawPoint(gRenderer, 2*x,2*y+1);
    SDL_RenderDrawPoint(gRenderer, 2*x+1,2*y+1);
#else
    SDL_RenderDrawPoint(gRenderer, x,y);             //1:1像素
#endif // 1

}



//---------------------------事件---------------------------------------------
int get_SDL_Event(void)       //键盘事件
{
    SDL_Event evt;
    while (SDL_PollEvent(&evt) != 0)
    {
        //event.type存储了当前的事件类型
        //如果无键盘鼠标 触摸点击 那么 默认是 0x200
        switch (evt.type)
        {
        case SDL_KEYDOWN:   //按下

            printf("\nk=%s",  SDL_GetKeyName(evt.key.keysym.sym));
            const Uint8 *state = SDL_GetKeyboardState(NULL);
            char k=0;
            if ( state[SDL_SCANCODE_UP])   k='U';
            if ( state[SDL_SCANCODE_DOWN] )k='D';
            if ( state[SDL_SCANCODE_LEFT] )k='L';
            if ( state[SDL_SCANCODE_RIGHT])k='R';

            if ( state[SDL_SCANCODE_0] )k='0';
            if ( state[SDL_SCANCODE_1] )k='1';
            if ( state[SDL_SCANCODE_2] )k='2';
            if ( state[SDL_SCANCODE_3] )k='3';
            if ( state[SDL_SCANCODE_4] )k='4';
            if ( state[SDL_SCANCODE_5] )k='5';
            if ( state[SDL_SCANCODE_6] )k='6';
            if ( state[SDL_SCANCODE_7] )k='7';
            if ( state[SDL_SCANCODE_8] )k='8';
            if ( state[SDL_SCANCODE_9] )k='9';

            if ( state[SDL_SCANCODE_A]) k='~';
            if ( state[SDL_SCANCODE_S]) k='+';
            if ( state[SDL_SCANCODE_D]) k='-';
            if ( state[SDL_SCANCODE_F]) k='*';
            if ( state[SDL_SCANCODE_G]) k='/';

            if ( state[SDL_SCANCODE_PAGEDOWN])  k='O';  //回车
            if ( state[SDL_SCANCODE_PAGEUP])    k='B';  //清除
            if ( state[SDL_SCANCODE_DELETE])    k='d';  //删除

        case SDL_KEYUP: //释放

            break;
        case SDL_TEXTINPUT:
            //如果是文本输入 输出文本
            // PrintText(event.text.text);
            break;
        case SDL_MOUSEBUTTONDOWN:
        /* 如果有任何鼠标点击消息 或者SDL_QUIT消息 那么将退出窗口 */
            g_input.is_pressed = 1;
            break;
        case SDL_MOUSEBUTTONUP:
             g_input.is_pressed = 0;
        break;
		case SDL_MOUSEMOTION:
			g_input.x = evt.motion.x;
			g_input.y = evt.motion.y;
		break;
        case SDL_QUIT:
            return -1;
            break;
        default:
            break;
        }
    }
    return 0;
}


void draw_disp_buff_cb(uint16_t* disp_buff, lui_area_t* area)
{
	uint8_t r,g,b;
	uint16_t i,j;

	for(j=area->y;j<=area->y + area->h - 1;j++)
	{
		for(i=area->x;i<= area->x + area->w - 1;i++)
		{
			r = (*disp_buff>>11)&0x1F;
			r<<=3;
			g = (*disp_buff>>5)&0x3F;
			g<<=2;
			b = (*disp_buff)&0x1F;
			b<<=3;
			g_disp_buffer_counter++;
			SDL_SetRenderDrawColor(gRenderer,(Uint8)r, (Uint8)g, (Uint8)b, 0xff);
			SDL_RenderDrawPoint(gRenderer, i,j);             //1:1像素
			 ++disp_buff;
			 if (g_disp_buffer_counter >= g_sidp_buffer_max_size)
			{
				SDL_RenderPresent(gRenderer);// 刷新窗口
				g_disp_buffer_counter = 0; //reset the counter
			}

		}
	}
    if (g_disp_buffer_counter >= g_sidp_buffer_max_size)
    {
        SDL_RenderPresent(gRenderer);// 刷新窗口
        g_disp_buffer_counter = 0; //reset the counter
    }

}

void my_input_read_sdl (lui_touch_input_data_t *input)
{
	input->is_pressed = g_input.is_pressed;
	input->y = g_input.y;
	input->x = g_input.x;
}


void lame_register_dispaly_driver(void)
{
	//----------------------------------------------------------
	//creating display driver variable for lame_ui
	lui_dispdrv_t *my_display_driver = lui_dispdrv_create();
	lui_dispdrv_register(my_display_driver);
	lui_dispdrv_set_resolution(my_display_driver, LCD_SCREEN_WIDTH, LCD_SCREEN_HEIGHT);
	lui_dispdrv_set_draw_disp_buff_cb(my_display_driver, draw_disp_buff_cb);
	lui_dispdrv_set_disp_buff (my_display_driver, disp_buffer, DISP_BUFF_PX_CNT);
}


void lame_register_input(void)
{
	//----------------------------------------------------------
	lui_touch_input_dev_t *my_input_device = lui_touch_inputdev_create();
	lui_touch_inputdev_register(my_input_device);
	lui_touch_inputdev_set_read_input_cb(my_input_device, my_input_read_sdl);
}
