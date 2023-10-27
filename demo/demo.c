#include "demo.h"

 void slider_event_handler_cb(lui_obj_t* obj)
 {
     if (lui_object_get_event(obj) == LUI_EVENT_VALUE_CHANGED)
     {
         int16_t val = lui_slider_get_value(obj);
         // Do something with this value..
     }
 }



void demo_create(void)
{
	//----------------------------------------------------------
	//create and add scenes
	lui_obj_t* scene_one = lui_scene_create();
	lui_scene_set_active(scene_one);
    lui_obj_t* chkbox_cricket = lui_checkbox_create();
    lui_checkbox_set_label_text(chkbox_cricket, "Cricket");
    lui_object_set_position(chkbox_cricket, 0, 0);
    lui_checkbox_set_value(chkbox_cricket, 0); // This checkbox is selected
    // Create a slider object with default knob
    lui_obj_t* slider_led_brightness = lui_slider_create();
    // Setting slider's area is important
    lui_object_set_area(slider_led_brightness, 160, 20);
    lui_object_set_position(slider_led_brightness, 0, 30);
    // Set range of slider 0-255
    lui_slider_set_range(slider_led_brightness, 0, 255);
    // Set default value of slider
    lui_slider_set_value(slider_led_brightness, 50);
    // Set callback function
    lui_object_set_callback(slider_led_brightness, slider_event_handler_cb);

    // Create a slider with text knob
    lui_obj_t* slider_2 = lui_slider_create();
    lui_object_set_area(slider_2, 160, 20);
    lui_object_set_position(slider_2, 0, 60);
    lui_slider_set_range(slider_2, 0, 100);
    lui_slider_set_value(slider_2, 69);
    // Set knob type as text
    lui_slider_set_knob_type(slider_2, LUI_SLIDER_KNOB_TYPE_TEXT);
    // We'll show slider's value on the knob, and also some custom text
    lui_slider_set_show_value(slider_2, 1);
    lui_slider_set_text(slider_2, " %");

	lui_object_add_to_parent(chkbox_cricket, scene_one);
	lui_object_add_to_parent(slider_led_brightness, scene_one);
	lui_object_add_to_parent(slider_2, scene_one);



}
