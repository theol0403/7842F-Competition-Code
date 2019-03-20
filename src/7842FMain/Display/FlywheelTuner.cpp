#include "FlywheelTuner.hpp"


FlywheelTuner::FlywheelTuner(lv_obj_t* parent) :
 bContainer(lv_obj_create(parent, NULL)), gContainer(lv_obj_create(parent, NULL))
{

  lv_color_t mainColor = LV_COLOR_HEX(0xFF7F00);

  /**
   * Button Bar
   */
  lv_obj_set_size(bContainer, lv_obj_get_width(parent), lv_obj_get_height(parent)/3);
  lv_obj_align(bContainer, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

  /**
   * Gauge Window
   */
  lv_obj_set_size(gContainer, lv_obj_get_width(parent), lv_obj_get_height(parent) - lv_obj_get_height(bContainer));
  lv_obj_align(gContainer, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

  lv_style_t* cStyle = new lv_style_t;
  lv_style_copy(cStyle, &lv_style_plain_color);
  cStyle->body.main_color = mainColor;
  cStyle->body.grad_color = mainColor;
  lv_obj_set_style(bContainer, cStyle);
  lv_obj_set_style(gContainer, cStyle);

}
