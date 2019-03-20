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
  std::map<std::string, button_t> buttons = {};

  std::vector<const char*> names = {};

  FlywheelTuner(lv_obj_t*);

  FlywheelTuner &withButton(std::string, double*, buttonType_t = buttonAdd, double = 1);
  void build();

};
