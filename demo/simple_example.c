#include "demo.h"
#include "res/forest.h"
#include "res/send_button.h"
#include "res/sent_button.h"
double g_points[50][2];
char name[50];
char addr[50];

lui_obj_t* scene_1;
lui_obj_t* label_1;
lui_obj_t* button_1;



void simple_example(void)
{
	/* ---------------------------------------------------------- */
	/* create and add scenes */
	scene_1 = lui_scene_create();
	lui_scene_set_active(scene_1);
	//lui_scene_set_bitmap_image(scene_1, &BITMAP_forest);

	label_1 = lui_label_create();
	lui_object_add_to_parent(label_1, scene_1);
	lui_object_set_position(label_1, 2, 15);
	lui_label_set_text(label_1, "We have 1 label and 2 buttons");
	lui_object_set_bg_color(label_1, lui_rgb(0, 0, 0));
	lui_label_set_bg_transparent(label_1, 0);
	lui_label_set_text_color(label_1, lui_rgb(255, 255, 255));

	button_1 = lui_button_create();
	lui_object_add_to_parent(button_1, scene_1);
	lui_object_set_position(button_1, 65, 75);
	lui_object_set_area(button_1, 110, 40);
	lui_button_set_label_texts(button_1, "Button 1", "Pressed!");
	lui_object_set_border_visibility(button_1, 1);

	lui_obj_t* label_2 = lui_label_create();
	lui_object_add_to_parent(label_2, scene_1);
	lui_object_set_position(label_2, 20, 125);
	lui_label_set_text(label_2, "Below is an image button");
	lui_label_set_text_color(label_2, lui_rgb(255, 200, 50));

	lui_obj_t* img_button = lui_button_create();
	lui_object_add_to_parent(img_button, scene_1);
	lui_object_set_area(img_button, 160, 49);
	lui_object_set_position(img_button, 40, 170);
	lui_button_set_bitmap_images(img_button, &BITMAP_send_button, &BITMAP_sent_button);
	lui_button_set_checkable(img_button, 1);

}
