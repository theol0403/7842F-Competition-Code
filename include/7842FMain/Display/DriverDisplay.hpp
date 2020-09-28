#pragma once
#include "main.h"

class DriverDisplay {

public:
  lv_obj_t* container = nullptr;
  lv_style_t style;

  DriverDisplay(lv_obj_t*);
  DriverDisplay(lv_obj_t*, lv_color_t);
  ~DriverDisplay();

  void setColor(lv_color_t);
};
