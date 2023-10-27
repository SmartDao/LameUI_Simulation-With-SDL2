#include "demo.h"


char txtbox_buff[50];
lui_obj_t* my_keyboard;
lui_obj_t* my_textbox;

// This callback is fired when user enters/exists the textbox.
// Here, we set the target textbox of the keyboard when user enters the textbox.
// This unhides the keyboard and textbox will receive inputs from the textbox.
// When user exits the textbox by closing the keyboard, keyboard gets hidden.
void textbox_callback(lui_obj_t* obj_txtbox)
{
    int8_t event = lui_object_get_event(obj_txtbox);
    if (event == LUI_EVENT_ENTERED)
    {
        lui_keyboard_set_target_txtbox(my_keyboard, my_textbox);
    }
    else if (event == LUI_EVENT_EXITED)
    {
        lui_keyboard_set_target_txtbox(my_keyboard, NULL);
    }
}

void widget_test(void)
{
	
	lui_obj_t* scene_one = lui_scene_create();
	lui_scene_set_active(scene_one);
	// Create a textbox object
	my_textbox = lui_textbox_create();
	// Important to set area of textbox.
	lui_object_set_area(my_textbox, 200, 40);
	//[Mandatory] Must set a buffer where the text will be stored
	lui_textbox_set_text_buffer(my_textbox, txtbox_buff, 40);
	// Let's set an initial string for the testbox.
	// Note: the size of string does NOT include the null (\0) terminating char.
	lui_textbox_insert_string(my_textbox, (char*)"Hello!!", 7);
	//[Important] Set the callback for textbox. In this callback, we will
	// link/unlink this textbox with a keyboard object. That will hide/unhide
	// the keyboard.
	lui_object_set_callback(my_textbox, textbox_callback);
	
	// Create a keyboard
	my_keyboard = lui_keyboard_create();
	// lui_object_set_height(my_keyboard, 200);
	// Keyboard is by default hidden. It is made visible when a
	// textbox is clicked on.


	lui_object_add_to_parent(my_textbox, scene_one);
	lui_object_add_to_parent(my_keyboard, scene_one);
	
}