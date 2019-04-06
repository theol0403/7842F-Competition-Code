#include "DriverDisplay.hpp"

DriverDisplay::DriverDisplay(lv_obj_t* parent) : DriverDisplay(parent, lv_obj_get_style(parent)->body.main_color, iangler) {}

DriverDisplay::DriverDisplay(lv_obj_t* parent, lv_color_t mainColor) :
container(lv_obj_create(parent, NULL))
{
  lv_obj_set_size(container, lv_obj_get_width(parent), lv_obj_get_height(parent));
  lv_obj_align(container, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

  lv_style_copy(&style, &lv_style_plain_color);
  style.body.main_color = mainColor;
  style.body.grad_color = mainColor;
  lv_obj_set_style(container, &style);
}

DriverDisplay::~DriverDisplay() {
  lv_obj_del(container);
}


void setColor(lv_color_t color) {
  style.body.main_color = color;
  style.body.grad_color = color;
  lv_obj_set_style(container, &style);
}
