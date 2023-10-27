#include "demo.h"
#include "../LameUI/fonts/ubuntu_regular_32.h"
#include "../LameUI/fonts/montserrat_regular_48.h"


lui_obj_t* g_scene_one;
lui_obj_t* g_scene_two;
lui_obj_t* g_lbl_cntr_value;
lui_obj_t* g_popup_panel;
lui_obj_t* g_popup_label;
lui_obj_t* g_grph;
lui_obj_t* g_btn_count;
lui_obj_t* g_btn_reset;
lui_obj_t* g_swtch_enable_bluetooth;
lui_obj_t* g_swtch_enable_wifi;
lui_obj_t* g_btn_nxt_scn;
lui_obj_t* g_btn_prev_scn;
lui_obj_t* g_lbl_slider1_val;
lui_obj_t* g_lbl_slider2_val;
lui_obj_t* slider1;
lui_obj_t* slider2;
lui_obj_t* keyboard;
lui_obj_t* btngrid_numpad;

lui_obj_t* g_active_txtbox = NULL;

#define CHART_POINTS	30	//721 max. After that core dump! Why?
double g_points[CHART_POINTS][2];
char g_btn_press_cnt[4] = {0};// For same reason as above
uint16_t g_btn_press_cnt_int = 0;


void lameui_input_read_cb (lui_touch_input_data_t *input);
void lameui_draw_disp_buff_cb (uint16_t* disp_buff, lui_area_t* area);

void count_and_reset_event_handler(lui_obj_t* obj);
void popupbtn_event_handler(lui_obj_t* obj);
void change_grph_event_handler(lui_obj_t* obj);
void enable_wifi_and_bt_event_handler(lui_obj_t* obj);
void slider_event_handler(lui_obj_t* obj);
void scn_change_event_handler(lui_obj_t* obj);
void textbox_callback1(lui_obj_t* obj_txtbox);
void list1_cb(lui_obj_t* obj);


void prepare_chart_data_1();
void prepare_chart_data_2(uint16_t val);

void big_example(void)
{
	g_scene_one = lui_scene_create();
	g_scene_two = lui_scene_create();
	lui_scene_set_active(g_scene_one);

	//----------------------------------------------------------
	//create label

	lui_obj_t* lbl_heading = lui_label_create();
	lui_object_set_area(lbl_heading, LCD_SCREEN_WIDTH, lui_gfx_get_font_height(&FONT_ubuntu_regular_32));
	lui_label_set_font(lbl_heading, &FONT_ubuntu_regular_32);
	lui_object_add_to_parent(lbl_heading, g_scene_one);
	lui_label_set_text(lbl_heading, "LameUI Demo Application");
	lui_object_set_position(lbl_heading, 0, 1);
	//lui_object_set_area(lbl_heading, HOR_RES, 20);
	lui_object_set_bg_color(lbl_heading, LUI_STYLE_BUTTON_BG_COLOR);
	lui_label_set_text_color(lbl_heading, lui_rgb(238, 238, 238));
	lui_object_set_border_width(lbl_heading, 1);


	//----------------------------------------------------------
	//Prepare some data for a graph
	prepare_chart_data_2(0);

	//----------------------------------------------------------
	//create a line chart with above data
	g_grph = lui_linechart_create();
	lui_object_add_to_parent(g_grph, g_scene_one);
	lui_linechart_set_data_source(g_grph, (double* )&g_points, CHART_POINTS);
// 	lui_linechart_set_data_range(g_grph, -1.2, 1.2);
	lui_object_set_position(g_grph, 20, 60);
	lui_object_set_area(g_grph, 440, 200);
	lui_linechart_set_point_color(g_grph, LUI_RGB(255, 0, 50));

	//----------------------------------------------------------

	lui_obj_t* lbl_cntr_txt = lui_label_create();
	lui_object_add_to_parent(lbl_cntr_txt, g_scene_one);
	lui_label_set_text(lbl_cntr_txt, "Counter:");
	lui_object_set_position(lbl_cntr_txt, 10, 285);
	lui_object_set_area(lbl_cntr_txt, 70, 20);


	g_lbl_cntr_value = lui_label_create();
	lui_object_add_to_parent(g_lbl_cntr_value, g_scene_one);
	lui_label_set_font(g_lbl_cntr_value, &FONT_montserrat_regular_48);
	lui_label_set_text(g_lbl_cntr_value, "0");
	lui_object_set_x_pos(g_lbl_cntr_value, 85);
	lui_object_set_y_pos(g_lbl_cntr_value, 270);
	lui_object_set_area(g_lbl_cntr_value, 110, 54);
	lui_object_set_border_width(g_lbl_cntr_value, 1);


	//create two buttons. One for count up, another for reset
	g_btn_count = lui_button_create();
	lui_object_add_to_parent(g_btn_count, g_scene_one);
	lui_object_set_position(g_btn_count, 230, 280);
	lui_object_set_area(g_btn_count, 100, 40);
	lui_button_set_label_text(g_btn_count, LUI_ICON_ADD " Count");
	lui_object_set_callback(g_btn_count, count_and_reset_event_handler);

	g_btn_reset = lui_button_create();
	lui_object_add_to_parent(g_btn_reset, g_scene_one);
	lui_object_set_position(g_btn_reset, 350, 280);
	lui_object_set_area(g_btn_reset, 100, 40);
	lui_button_set_label_text(g_btn_reset, LUI_ICON_RELOAD " Reset");
	lui_object_set_callback(g_btn_reset, count_and_reset_event_handler);


}


// Event handler for button, called back by LameUI
void count_and_reset_event_handler(lui_obj_t* obj)
{
	//printf("\nState Change occured. Event ID: %d", event);
	//memset(event_name, 0, strlen(event_name));
	uint8_t event = lui_object_get_event(obj);
	if (event ==  LUI_EVENT_PRESSED)
	{
		if (obj == g_btn_count)
		{
			sprintf(g_btn_press_cnt, "%d", ++g_btn_press_cnt_int);
			lui_label_set_text(g_lbl_cntr_value, g_btn_press_cnt);
		}
		else /* if (obj == g_btn_reset) */
		{
			g_btn_press_cnt_int = 0;
			sprintf(g_btn_press_cnt, "%d", g_btn_press_cnt_int);
			lui_label_set_text(g_lbl_cntr_value, g_btn_press_cnt);
		}

		prepare_chart_data_2(g_btn_press_cnt_int);
		lui_linechart_set_data_source(g_grph, (double* )&g_points, CHART_POINTS);
	}
}

void textbox_callback1(lui_obj_t* obj_txtbox)
{
	uint8_t event = lui_object_get_event(obj_txtbox);
	if (event == LUI_EVENT_ENTERED)
	{
		// lui_keyboard_set_target_txtbox(keyboard, obj_txtbox);
		/* Exit from previously active textbox (if any) */
		lui_textbox_exit_edit_mode(g_active_txtbox);
		g_active_txtbox = obj_txtbox;	// for physical keyboard
	}
	else if (event == LUI_EVENT_EXITED)
	{
		lui_keyboard_set_target_txtbox(keyboard, NULL);
		g_active_txtbox = NULL;		// for physical keyboard
	}
}

// Event handler for popup buttons
void popupbtn_event_handler(lui_obj_t* obj)
{
	//printf("\nState Change occured. Event ID: %d", event);
	//memset(event_name, 0, strlen(event_name));
	uint8_t event = lui_object_get_event(obj);
	switch (event)
	{
	case LUI_EVENT_PRESSED:
		lui_object_set_visibility(g_popup_panel, 0);
		fprintf(stderr, "Invisible!\n");
		break;
	default:
		break;
	}
}


// Event handler for switch change graph. Called back by LameUI
void enable_wifi_and_bt_event_handler(lui_obj_t* obj)
{
	// change background color
	uint8_t event = lui_object_get_event(obj);
	if (event == LUI_EVENT_VALUE_CHANGED)
	{
		uint8_t val = lui_switch_get_value(obj);
		if (obj == g_swtch_enable_wifi)
		{
			if (val == 1)
			{
				lui_label_set_text(g_popup_label, "Alert:\nWiFi is enabled");
				lui_object_set_visibility(g_popup_panel, 1);
				fprintf(stderr, "Visible!\n");
			}
			else
			{
				lui_label_set_text(g_popup_label, "Alert:\nWiFi is disabled");
				lui_object_set_visibility(g_popup_panel, 1);
				fprintf(stderr, "Visible!\n");
			}
		}
		else if (obj == g_swtch_enable_bluetooth)
		{
			if (val == 1)
			{
				lui_label_set_text(g_popup_label, "Alert:\nBluetooth is enabled");
				lui_object_set_visibility(g_popup_panel, 1);
				fprintf(stderr, "Visible!\n");
			}
			else
			{
				lui_label_set_text(g_popup_label, "Alert:\nBluetooth is disabled");
				lui_object_set_visibility(g_popup_panel, 1);
				fprintf(stderr, "Visible!\n");
			}
		}
	}

}

// Event handler for slider. Changes a label's text based on slider value
void slider_event_handler(lui_obj_t* obj)
{
	if (lui_object_get_event(obj) == LUI_EVENT_VALUE_CHANGED)
	{
		int16_t val = lui_slider_get_value(obj);
		if (obj == slider1)
		{
			static char buf_slider1[5]; /* max 3 bytes for number plus 1 sign plus 1 null terminating byte */
			snprintf(buf_slider1, 5, "%d", val);
			lui_label_set_text(g_lbl_slider1_val, buf_slider1);
		}
		else if (obj == slider2)
		{
			static char buf_slider2[5]; /* max 3 bytes for number plus 1 sign plus 1 null terminating byte */
			snprintf(buf_slider2, 5, "%d", val);
			lui_label_set_text(g_lbl_slider2_val, buf_slider2);
		}

	}
}

// Event handler for next_scene button and prev_scene button press
void scn_change_event_handler(lui_obj_t* obj)
{
	uint8_t event = lui_object_get_event(obj);

	if (event == LUI_EVENT_PRESSED)
	{
		if (obj == g_btn_nxt_scn)
		{
			lui_scene_set_active(g_scene_two);
		}
		else if (obj == g_btn_prev_scn)
		{
			lui_scene_set_active(g_scene_one);
		}

	}
}

void list1_cb(lui_obj_t* obj)
{
	uint8_t event = lui_object_get_event(obj);
	if (event == LUI_EVENT_PRESSED)
	{
		const char* txt = lui_list_get_item_text(obj, lui_list_get_selected_item_index(obj));
		if (txt != NULL)
			fprintf(stderr, "Selected: %s\n", txt);
	}
}

void prepare_chart_data_2(uint16_t sin_val)
{
	double deg_to_rad = 3.14159265 / 180.0;

	for (int i = 0; i < CHART_POINTS; i++)
	{
		g_points[i][0] = i*i;		//x value of i'th element

		g_points[i][1] = sin((double)i * deg_to_rad * (double)sin_val);	//y value of i'th element

	}

}


