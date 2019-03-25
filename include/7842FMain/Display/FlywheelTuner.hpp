#pragma once
#include "main.h"
#include <map>

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

  static lv_color_t needleColors[4];

  lv_obj_t* container = nullptr;
  lv_color_t mainColor;

  pros::Task task;

  using ButtonGroup = std::tuple<std::string, button_t, lv_obj_t*>;
  std::vector<ButtonGroup> buttons = {};
  double multiplier = 1;

  std::vector<std::tuple<std::string, std::vector<double*>, lv_obj_t*>> gauges = {};

  FlywheelTuner(lv_obj_t*);
  FlywheelTuner(lv_obj_t*, lv_color_t);
  ~FlywheelTuner();

  FlywheelTuner &withButton(std::string, double*, btnType = btnType::add, double = 1);
  FlywheelTuner &withGauge(std::string, std::vector<double*>, int, int);
  void build();

  void calcLabels();

  static lv_res_t btnAction(lv_obj_t*, const char*);

  static void taskFnc(void*);

};
