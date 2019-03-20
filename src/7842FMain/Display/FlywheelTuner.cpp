#include "FlywheelTuner.hpp"


FlywheelTuner::FlywheelTuner(lv_obj_t* parent) :
container(lv_obj_create(parent, NULL))
{

  lv_color_t mainColor = LV_COLOR_HEX(0xFF7F00);

  /**
  * Container
  */
  lv_obj_set_size(container, lv_obj_get_width(parent), lv_obj_get_height(parent));
  lv_obj_align(container, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

  lv_style_t* cStyle = new lv_style_t;
  lv_style_copy(cStyle, &lv_style_plain_color);
  cStyle->body.main_color = mainColor;
  cStyle->body.grad_color = mainColor;
  lv_obj_set_style(container, cStyle);
  lv_obj_set_style(container, cStyle);

}


FlywheelTuner &FlywheelTuner::withButton(std::string name, double* variable, buttonType_t type, double modifier) {
  buttons.insert_or_assign(name, button_t{variable, type, modifier});
  return *this;
}
