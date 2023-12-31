#include "demo.h"
#include "../LameUI/fonts/montserrat_regular_32.h"
#include "../LameUI/fonts/ubuntu_regular_32..h"
#include "../LameUI/fonts/ubuntu_bold_20.h"
#include "../LameUI/fonts/montserrat_bold_20.h"

#include "res/back.h"
#include "res/next.h"
#include "res/shuffle.h"
#include "res/play.h"
#include "res/pause.h"
#include "res/repeat.h"
#include "res/heart_filled.h"
#include "res/heart_line.h"
#include "res/playlist.h"
#include "res/info.h"
#include "res/close.h"
#include "res/lastofus.h"
#include "res/background.h"
#include "res/popup_bckgrnd.h"

struct song_info {
    char* title;
    char* artist;
    char* album;
    uint16_t duration;  // in seconds
    uint8_t is_fav;
};

struct song_info song_list[] = {
    {.title="The Path", .album="The Last of Us", .artist="Gustavo Santaolalla", .duration=88, .is_fav=1},
    {.title="All Gone (Alone)", .album="The Last of Us", .artist="Gustavo Santaolalla", .duration=82, .is_fav=0},
    {.title="Blackout", .album="The Last of Us", .artist="Gustavo Santaolalla", .duration=98, .is_fav=0},
    {.title="The Last of Us", .album="The Last of Us", .artist="Gustavo Santaolalla", .duration=183, .is_fav=1},
    {.title="Vanishing Grace", .album="The Last of Us", .artist="Gustavo Santaolalla", .duration=126, .is_fav=0},
    {.title="The Hunters", .album="The Last of Us", .artist="Gustavo Santaolalla", .duration=119, .is_fav=1},
    {.title="By Any Means", .album="The Last of Us", .artist="Gustavo Santaolalla", .duration=113, .is_fav=0},
    {.title="The Choice", .album="The Last of Us", .artist="Gustavo Santaolalla", .duration=102, .is_fav=1},
};

uint8_t songs_count =  sizeof(song_list) / sizeof(song_list[0]);
struct song_info* current_song;
uint8_t current_song_index = 0;
uint16_t current_song_timer = 120;
uint8_t is_playing = 0;
uint8_t is_popup_vis = 0;

char time_ela_str[6];   // buffer to store song elapsed string in mm:ss format
char time_tot_str[6];   // buffer to store song total time

lui_obj_t* scn_main;
lui_obj_t* scn_second;
lui_obj_t* lbl_song;
lui_obj_t* lbl_artist;
lui_obj_t* lbl_album;
lui_obj_t* lbl_time_ela;
lui_obj_t* lbl_time_tot;
lui_obj_t* lbl_about1;
lui_obj_t* lbl_about2;

lui_obj_t* btn_playlst;
lui_obj_t* btn_about;
lui_obj_t* btn_fav;
lui_obj_t* btn_play;
lui_obj_t* btn_prev;
lui_obj_t* btn_next;
lui_obj_t* btn_shfl;
lui_obj_t* btn_rpt;
lui_obj_t* btn_home;
lui_obj_t* btn_srch;
lui_obj_t* btn_popup_cls;

lui_obj_t* lst_songs;

lui_obj_t* sldr_prog;

lui_obj_t* pnl_img;
lui_obj_t* pnl_about_popup;

void song_progress_timer_cb(int val);
void play_btn_cb(lui_obj_t* obj);
void nav_btn_cb(lui_obj_t* obj);
void playlst_btn_cb(lui_obj_t* obj);
void about_btn_cb(lui_obj_t* obj);
void popup_cls_btn_cb(lui_obj_t* obj);
void song_list_cb(lui_obj_t* obj);
void fav_btn_cb(lui_obj_t* obj);
void slider_change_cb(lui_obj_t* obj);

void format_time_str(uint16_t duration, char* time_ela_str);
void play_next_song();
void play_prev_song();
void set_current_song(uint8_t song_index);

void music_demo(void)
{
/* ---------------------------------------------------------- */
	/* create and add scenes */
	scn_main = lui_scene_create();
	lui_scene_set_active(scn_main);
    lui_scene_set_bitmap_image(scn_main, &BITMAP_background);

    scn_second = lui_scene_create();
    // lui_scene_set_bitmap_image(scn_second, &BITMAP_background);
	// lui_scene_set_active(scn_second);

    current_song = &song_list[current_song_index];
    /* Labels */
    {

        lbl_song = lui_label_create();
        lui_object_add_to_parent(lbl_song, scn_main);
        uint16_t x = 125, y = 43;
        lui_object_set_position(lbl_song, x, y);
        lui_label_set_text(lbl_song, current_song->title);
        lui_label_set_font(lbl_song, &FONT_ubuntu_bold_20);
        lui_label_set_text_color(lbl_song, lui_rgb(0, 0, 0));
        lui_object_set_width(lbl_song, LCD_SCREEN_WIDTH-x);

        lbl_artist = lui_label_create();
        lui_object_add_to_parent(lbl_artist, scn_main);
        y = 70;
        lui_object_set_position(lbl_artist, x, y);
        lui_label_set_text(lbl_artist, current_song->artist);
        lui_label_set_text_color(lbl_artist, lui_rgb(0, 0, 0));
        lui_object_set_width(lbl_album, LCD_SCREEN_WIDTH-x);

        lbl_album = lui_label_create();
        lui_object_add_to_parent(lbl_album, scn_main);
        y = 90;
        lui_object_set_position(lbl_album, x, y);
        lui_label_set_text(lbl_album, current_song->album);
        lui_label_set_text_color(lbl_album, lui_rgb(0, 0, 0));
        lui_object_set_width(lbl_album, LCD_SCREEN_WIDTH-x);
    }

    /* All buttons, sliders */
    {
        lui_bitmap_mono_pal_t nxt_idle_pal = {
            .fore_color = lui_rgb(50, 50, 50),
            .back_color = 0,
            .is_backgrnd = 0
        };
        lui_bitmap_mono_pal_t nxt_press_pal = {
            .fore_color = lui_rgb(120, 0, 250),
            .back_color = 0,
            .is_backgrnd = 0
        };

        lui_bitmap_mono_pal_t heart_idle_pal = {
            .fore_color = lui_rgb(170, 170,170),
            .back_color = 0,
            .is_backgrnd = 0
        };
        lui_bitmap_mono_pal_t hear_press_pal = {
            .fore_color = lui_rgb(255, 0, 0),
            .back_color = 0,
            .is_backgrnd = 0
        };

        btn_about = lui_button_create();
        lui_object_add_to_parent(btn_about, scn_main);
        lui_object_set_position(btn_about, 10, 5);
        lui_object_set_area(btn_about, 30, 30);
        lui_button_set_bitmap_images(btn_about, &BITMAP_info, &BITMAP_info);
        lui_button_set_bitmap_images_mono_palette(btn_about, &nxt_idle_pal, &nxt_press_pal);
        lui_button_set_bg_transparent(btn_about, 1);
        lui_object_set_callback(btn_about, about_btn_cb);

        btn_playlst = lui_button_create();
        lui_object_add_to_parent(btn_playlst, scn_main);
        lui_object_set_position(btn_playlst, 280, 5);
        lui_object_set_area(btn_playlst, 30, 30);
        lui_button_set_bitmap_images(btn_playlst, &BITMAP_playlist, &BITMAP_playlist);
        lui_button_set_bitmap_images_mono_palette(btn_playlst, &nxt_idle_pal, &nxt_press_pal);
        lui_button_set_bg_transparent(btn_playlst, 1);
        lui_object_set_callback(btn_playlst, playlst_btn_cb);

        btn_fav = lui_button_create();
        lui_object_add_to_parent(btn_fav, scn_main);
        lui_object_set_position(btn_fav, 125, 113);
        lui_object_set_area(btn_fav, 28, 28);
        lui_button_set_bitmap_images(btn_fav, &BITMAP_heart_line, &BITMAP_heart_filled);
        lui_button_set_bitmap_images_mono_palette(btn_fav, &heart_idle_pal, &hear_press_pal);
        lui_button_set_checkable(btn_fav, 1);
        lui_button_set_value(btn_fav, current_song->is_fav);
        lui_button_set_bg_transparent(btn_fav, 1);
        lui_object_set_callback(btn_fav, fav_btn_cb);

        btn_play = lui_button_create();
        lui_object_add_to_parent(btn_play, scn_main);
        lui_object_set_position(btn_play, 130, 178);
        lui_object_set_area(btn_play, 60, 60);
        lui_button_set_bitmap_images(btn_play, &BITMAP_play, &BITMAP_pause);
        lui_button_set_bitmap_images_mono_palette(btn_play, &nxt_press_pal, &nxt_idle_pal); // opposite palette for play button
        lui_button_set_checkable(btn_play, 1);
        lui_button_set_bg_transparent(btn_play, 1);
        lui_object_set_callback(btn_play, play_btn_cb);

        btn_next = lui_button_create();
        lui_object_add_to_parent(btn_next, scn_main);
        lui_object_set_position(btn_next, 210, 193);
        lui_object_set_area(btn_next, 35, 35);
        lui_button_set_bitmap_images(btn_next, &BITMAP_next, &BITMAP_next);
        lui_button_set_bitmap_images_mono_palette(btn_next, &nxt_idle_pal, &nxt_press_pal);
        lui_button_set_bg_transparent(btn_next, 1);
        lui_object_set_callback(btn_next, nav_btn_cb);

        btn_shfl = lui_button_create();
        lui_object_add_to_parent(btn_shfl, scn_main);
        lui_object_set_position(btn_shfl, 270, 193);
        lui_object_set_area(btn_shfl, 35, 35);
        lui_button_set_bitmap_images(btn_shfl, &BITMAP_shuffle, &BITMAP_shuffle);
        lui_button_set_bitmap_images_mono_palette(btn_shfl, &nxt_idle_pal, &nxt_press_pal);
        lui_button_set_checkable(btn_shfl, 1);
        lui_button_set_bg_transparent(btn_shfl, 1);

        btn_prev = lui_button_create();
        lui_object_add_to_parent(btn_prev, scn_main);
        lui_object_set_position(btn_prev, 70, 193);
        lui_object_set_area(btn_prev, 35, 35);
        lui_button_set_bitmap_images(btn_prev, &BITMAP_back, &BITMAP_back);
        lui_button_set_bitmap_images_mono_palette(btn_prev, &nxt_idle_pal, &nxt_press_pal);
        lui_button_set_bg_transparent(btn_prev, 1);
        lui_object_set_callback(btn_prev, nav_btn_cb);

        btn_rpt = lui_button_create();
        lui_object_add_to_parent(btn_rpt, scn_main);
        lui_object_set_position(btn_rpt, 10, 193);
        lui_object_set_area(btn_rpt, 35, 35);
        lui_button_set_bitmap_images(btn_rpt, &BITMAP_repeat, &BITMAP_repeat);
        lui_button_set_bitmap_images_mono_palette(btn_rpt, &nxt_idle_pal, &nxt_press_pal);
        lui_button_set_checkable(btn_rpt, 1);
        lui_button_set_bg_transparent(btn_rpt, 1);


    }

    /* All panels (and their children) */
    {
        pnl_img = lui_panel_create();
        lui_object_add_to_parent(pnl_img, scn_main);
        lui_object_set_position(pnl_img, 10, 37);
        lui_object_set_area(pnl_img, 104, 104);
        lui_panel_set_bitmap_image(pnl_img, &BITMAP_lastofus);
        lui_object_set_border_visibility(pnl_img, 0);

        /* Popup panel */
        {
            pnl_about_popup = lui_panel_create();
            lui_object_add_to_parent(pnl_about_popup, scn_main);
            lui_object_set_position(pnl_about_popup, 30, 30);
            lui_object_set_area(pnl_about_popup, 260, 180);
            lui_object_set_layer(pnl_about_popup, LUI_LAYER_POPUP);
            lui_object_set_border_visibility(pnl_about_popup, 0);
            lui_panel_set_bitmap_image(pnl_about_popup, &BITMAP_popup_bckgrnd);
            lui_object_set_visibility(pnl_about_popup, 0);

            lbl_about1 = lui_label_create();
            lui_object_add_to_parent(lbl_about1, pnl_about_popup);
            lui_object_set_position(lbl_about1, 10, 20);
            lui_label_set_text(lbl_about1, "Music Player demo using LameUI v2.0");
            lui_label_set_font(lbl_about1, &FONT_ubuntu_bold_20);
            lui_label_set_text_color(lbl_about1, lui_rgb(120, 60, 180));
            lui_object_set_width(lbl_about1, 240);

            lbl_about2 = lui_label_create();
            lui_object_add_to_parent(lbl_about2, pnl_about_popup);
            lui_object_set_position(lbl_about2, 10, 80);
            lui_label_set_text(lbl_about2, "github.com/abhra0897/LameUI");
            // lui_label_set_text_color(lbl_about2, lui_rgb(0, 0, 0));
            lui_object_set_width(lbl_about2, 240);

            lui_bitmap_mono_pal_t popup_cls_pal = {
                .back_color = 0,
                .fore_color = lui_rgb(255, 255, 255),
                .is_backgrnd = 0
            };

            btn_popup_cls = lui_button_create();
            lui_object_add_to_parent(btn_popup_cls, pnl_about_popup);
            lui_object_set_position(btn_popup_cls, 117, 140);
            lui_object_set_area(btn_popup_cls, 35, 35);
            lui_button_set_bitmap_images(btn_popup_cls, &BITMAP_close, &BITMAP_close);
            lui_button_set_bitmap_images_mono_palette(btn_popup_cls, &popup_cls_pal, &popup_cls_pal);
            lui_button_set_bg_transparent(btn_popup_cls, 1);
            lui_object_set_callback(btn_popup_cls, popup_cls_btn_cb);
        }
    }

    /* Slider object */
    {
        sldr_prog = lui_slider_create();
        lui_object_add_to_parent(sldr_prog, scn_main);
        lui_object_set_position(sldr_prog, 5, 158);
        lui_object_set_area(sldr_prog, 300, 5);
        lui_slider_set_knob_type(sldr_prog, LUI_SLIDER_KNOB_TYPE_NONE);
        lui_object_set_border_visibility(sldr_prog, 0);
        lui_object_set_bg_color(sldr_prog, lui_rgb(220, 220, 250));
        lui_slider_set_extra_colors(sldr_prog, 0, lui_rgb(120, 0, 250), 0);
        lui_slider_set_range(sldr_prog, 0, current_song->duration);
        lui_slider_set_value(sldr_prog, 0);
        lui_object_set_callback(sldr_prog, slider_change_cb);

        lbl_time_ela = lui_label_create();
        lui_object_add_to_parent(lbl_time_ela, scn_main);
        lui_object_set_position(lbl_time_ela, 10, 164);
        lui_label_set_text(lbl_time_ela, "00:00");
        lui_label_set_text_color(lbl_time_ela, lui_rgb(0, 0, 0));
        /* as this label changes its value, set a bigger area manually for it */
        uint16_t lbl_dim1[2];
        lui_gfx_get_string_dimension("0:00:00", &LUI_DEFAULT_FONT, LCD_SCREEN_WIDTH, lbl_dim1);
        lui_object_set_area(lbl_time_ela, lbl_dim1[0], lbl_dim1[1]);

        lbl_time_tot = lui_label_create();
        lui_object_add_to_parent(lbl_time_tot, scn_main);
        lui_object_set_position(lbl_time_tot, 275, 164);
        format_time_str(current_song->duration, time_tot_str);
        lui_label_set_text(lbl_time_tot, time_tot_str);
        lui_label_set_text_color(lbl_time_tot, lui_rgb(0, 0, 0));
        lui_object_set_area(lbl_time_tot, lbl_dim1[0], lbl_dim1[1]);

    }

    {
        lst_songs = lui_list_create();
        lui_object_add_to_parent(lst_songs, scn_second);
        lui_object_set_area(lst_songs, 300, 210);
        lui_object_set_position(lst_songs, 10, 20);
        lui_list_set_max_items_count(lst_songs, 20);
        for (int i = 0; i < songs_count; i++)
        {
            char* temp_buff = malloc(512 * sizeof(*temp_buff));
            sprintf(temp_buff, "%d. %s", i+1, song_list[i].title);
            lui_list_add_item(lst_songs, temp_buff);
        }
        lui_object_set_callback(lst_songs, song_list_cb);
        lui_list_prepare(lst_songs);
    }



}


void song_list_cb(lui_obj_t* obj)
{
    uint8_t event = lui_object_get_event(obj);
	if (event != LUI_EVENT_RELEASED)
        return;
    int16_t sel_index = lui_list_get_selected_item_index(obj);
    if (sel_index < 0)
        return;
    lui_scene_set_active(scn_main);
    set_current_song(sel_index);
}

void play_btn_cb(lui_obj_t* obj)
{
    uint8_t chk = lui_button_get_check_value(obj);
    uint8_t event = lui_object_get_event(obj);

    if (event != LUI_EVENT_VALUE_CHANGED)
        return;

    is_playing = chk;

}

void nav_btn_cb(lui_obj_t* obj)
{
    uint8_t event = lui_object_get_event(obj);
    // fprintf(stderr, "ev: %d PRESSED: %d, True/False: %d\n", event, LUI_EVENT_PRESSED, event==LUI_EVENT_PRESSED);
    if (event != LUI_EVENT_RELEASED)
        return;
    if (obj == btn_next)
        play_next_song();
    else if (obj == btn_prev)
        play_prev_song();

}

void playlst_btn_cb(lui_obj_t* obj)
{
    uint8_t event = lui_object_get_event(obj);
    if (event != LUI_EVENT_RELEASED)
        return;
    lui_scene_set_active(scn_second);
}

void fav_btn_cb(lui_obj_t* obj)
{
    uint8_t event = lui_object_get_event(obj);
    if (event != LUI_EVENT_VALUE_CHANGED)
        return;
    uint8_t chk = lui_button_get_check_value(obj);
    current_song->is_fav = chk;
}

void about_btn_cb(lui_obj_t* obj)
{
    uint8_t event = lui_object_get_event(obj);
    if (event != LUI_EVENT_RELEASED)
        return;
    lui_object_set_visibility(pnl_about_popup, 1);
    is_popup_vis = 1;
}

void popup_cls_btn_cb(lui_obj_t* obj)
{
    uint8_t event = lui_object_get_event(obj);
    if (event != LUI_EVENT_RELEASED)
        return;
    lui_object_set_visibility(pnl_about_popup, 0);
    is_popup_vis = 0;
}

void slider_change_cb(lui_obj_t* obj)
{
    uint8_t event = lui_object_get_event(obj);
    if (event != LUI_EVENT_VALUE_CHANGED)
        return;
    int16_t val = lui_slider_get_value(obj);
    current_song_timer = val;
    format_time_str(current_song_timer, time_ela_str);
    lui_label_set_text(lbl_time_ela, time_ela_str);
}

void play_next_song()
{
    if (current_song_index >= songs_count - 1)
        current_song_index = 0;
    else
        ++current_song_index;
    set_current_song(current_song_index);
}

void play_prev_song()
{
    if (current_song_index == 0)
        current_song_index = songs_count - 1;
    else
        --current_song_index;
    set_current_song(current_song_index);
}

void song_progress_timer_cb(int val)
{

    if (!is_playing)
        return;
    if (current_song_timer+1 > current_song->duration)
    {
        play_next_song();
        return;
    }
    ++current_song_timer;
    format_time_str(current_song_timer, time_ela_str);
    lui_label_set_text(lbl_time_ela, time_ela_str);
    lui_slider_set_value(sldr_prog, current_song_timer);

    /* Important Patch: Rendering anything below a popup layer wil draw over the popup.
       So, we've to redraw the popup manually. We'll set `need_refresh` flag for the popup
       so it renders.
       TODO: Handle such incident in library*/
    if (is_popup_vis)
        _lui_object_set_need_refresh(pnl_about_popup);
}

void set_current_song(uint8_t song_index)
{
    current_song = &song_list[song_index];
    current_song_timer = 0;
    is_playing = 1;
    format_time_str(current_song->duration, time_tot_str);
    lui_label_set_text(lbl_time_tot, time_tot_str);
    format_time_str(0, time_ela_str);
    lui_label_set_text(lbl_time_ela, time_ela_str);
    lui_label_set_text(lbl_song, current_song->title);
    lui_label_set_text(lbl_artist, current_song->artist);
    lui_label_set_text(lbl_album, current_song->album);
    lui_button_set_checked(btn_play);
    lui_button_set_value(btn_fav, current_song->is_fav);
    lui_slider_set_range(sldr_prog, 0, current_song->duration);
    lui_slider_set_value(sldr_prog, 0);
}

void format_time_str(uint16_t duration, char* time_ela_str)
{
    if (!time_ela_str)
        return;
    uint8_t min = duration / 60;
    uint8_t sec = duration % 60;
    snprintf(time_ela_str, 6, "%02d:%02d", min, sec);
}


