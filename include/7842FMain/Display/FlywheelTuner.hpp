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

  static std::vector<lv_color_t> needleColors;

  lv_obj_t *container = nullptr;
  lv_color_t mainColor;

  std::vector<std::tuple<std::string, button_t, lv_obj_t*>> buttons = {};
  double multiplier = 1;

  std::vector<std::tuple<std::string, std::vector<double*>, lv_obj_t*>> gauges = {};
  pros::Task* gaugeTask = nullptr;

  FlywheelTuner(lv_obj_t*);
  ~FlywheelTuner();

  FlywheelTuner &withButton(std::string, double*, btnType = btnType::add, double = 1);
  FlywheelTuner &withGauge(std::string, std::vector<double*>, int, int);
  void build();

  void calcLabels();

  static lv_res_t btnAction(lv_obj_t*, const char*);
  static void gaugeLoop(void*);

};
