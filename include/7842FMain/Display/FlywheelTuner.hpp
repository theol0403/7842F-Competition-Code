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
    double* variable;
    buttonType_t buttonType = buttonAdd;
    int modifier = 1;
  }

  std::map<std::string, button_t> buttons;

  lv_obj_t *bContainer = nullptr;
  lv_obj_t *gContainer = nullptr;



  FlywheelTuner(lv_obj_t*);

  FlywheelTuner &withButton(std::string, double*, buttonType_t);



};
