// #include "main.h"
//
// lv_obj_t * objectContainer;
// lv_style_t blueObjectStyle;
// lv_style_t redObjectStyle;
// lv_style_t objectStyle;
//
// void initObjectStyles() // Run at init to create the styles and main screen
// {
// // Background ----------------------------------------------------------------------
// objectContainer = lv_obj_create(lv_scr_act(), NULL);
// lv_obj_set_size(objectContainer, display_C::OBJECT_CONTAINER_WIDTH, display_C::OBJECT_CONTAINER_HEIGHT);
// lv_obj_align(objectContainer, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
// //lv_obj_set_pos(objectContainer, 0, 0);
//
// // Style for background of screen
// static lv_style_t backgroundStyle;
// lv_style_copy(&backgroundStyle, &lv_style_plain_color);
// backgroundStyle.body.main_color = LV_COLOR_GRAY;
// lv_obj_set_style(objectContainer, &backgroundStyle);
// // Background ----------------------------------------------------------------------
//
//
// // Object Sytles ----------------------------------------------------------------------
// //Flag Object Style
// lv_style_copy(&objectStyle, &lv_style_pretty_color);
// objectStyle.body.main_color = LV_COLOR_GREEN;
// objectStyle.body.grad_color = LV_COLOR_GREEN;
// objectStyle.body.radius = 8;
// objectStyle.body.border.color = LV_COLOR_GREEN;
// objectStyle.body.border.width = 3;
// objectStyle.body.border.opa = LV_OPA_100;
//
// //Blue object style
// lv_style_copy(&blueObjectStyle, &objectStyle);
// blueObjectStyle.body.main_color = LV_COLOR_BLUE;
// blueObjectStyle.body.grad_color = LV_COLOR_BLUE;
//
// //Red object style
// lv_style_copy(&redObjectStyle, &objectStyle);
// redObjectStyle.body.main_color = LV_COLOR_RED;
// redObjectStyle.body.grad_color = LV_COLOR_RED;
// // Object Sytles ----------------------------------------------------------------------
//
// }
