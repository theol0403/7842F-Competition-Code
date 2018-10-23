// #include "main.h"
//
// lv_obj_t * objectContainer;
// lv_style_t m_blueSimpleObjectStyle;
// lv_style_t m_redSimpleObjectStyle;
// lv_style_t m_simpleObjectStyle;
//
// void initm_simpleObjectStyles() // Run at init to create the styles and main screen
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
// lv_style_copy(&m_simpleObjectStyle, &lv_style_pretty_color);
// m_simpleObjectStyle.body.main_color = LV_COLOR_GREEN;
// m_simpleObjectStyle.body.grad_color = LV_COLOR_GREEN;
// m_simpleObjectStyle.body.radius = 8;
// m_simpleObjectStyle.body.border.color = LV_COLOR_GREEN;
// m_simpleObjectStyle.body.border.width = 3;
// m_simpleObjectStyle.body.border.opa = LV_OPA_100;
//
// //Blue object style
// lv_style_copy(&m_blueSimpleObjectStyle, &m_simpleObjectStyle);
// m_blueSimpleObjectStyle.body.main_color = LV_COLOR_BLUE;
// m_blueSimpleObjectStyle.body.grad_color = LV_COLOR_BLUE;
//
// //Red object style
// lv_style_copy(&m_redSimpleObjectStyle, &m_simpleObjectStyle);
// m_redSimpleObjectStyle.body.main_color = LV_COLOR_RED;
// m_redSimpleObjectStyle.body.grad_color = LV_COLOR_RED;
// // Object Sytles ----------------------------------------------------------------------
//
// }
