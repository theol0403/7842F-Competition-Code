#pragma once
#include "main.h"
#include <map>
#include <sstream>
#include <iomanip>

class FlywheelTuner
{

public:

  enum class btnType {
    add,
    multiply,
    increment
  };

  struct button_t {
    double* variable = nullptr;
    btnType type = btnType::add;
    double modifier = 1;
  };

  lv_obj_t *container = nullptr;
  std::vector<std::tuple<std::string, button_t, lv_obj_t*>> buttons = {};
  double multiplier = 1;

  FlywheelTuner(lv_obj_t*);
  ~FlywheelTuner();

  FlywheelTuner &withButton(std::string, double*, btnType = btnType::add, double = 1);
  void build();

  void calcLabels();

  static lv_res_t btnAction(lv_obj_t*, const char*);

};
