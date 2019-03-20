#pragma once
#include "main.h"
#include <map>

class FlywheelTuner
{

public:

  enum buttonType_t {
    buttonAdd,
    buttonMultiply,
    buttonIncrement
  };

  struct button_t {
    double* variable = nullptr;
    buttonType_t type = buttonAdd;
    double modifier = 1;
  };

  lv_obj_t *container = nullptr;
  lv_color_t mainColor;
  std::vector<std::pair<std::string, button_t>> buttons = {};

  FlywheelTuner(lv_obj_t*);
  ~FlywheelTuner();

  FlywheelTuner &withButton(std::string, double*, buttonType_t = buttonAdd, double = 1);
  void build();

  static lv_res_t btnAction(lv_obj_t*, const char*);

};
