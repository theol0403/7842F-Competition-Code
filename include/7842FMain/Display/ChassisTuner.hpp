#pragma once
#include "main.h"
#include "lib7842/odometry/controller.hpp"

class ChassisTuner
{

public:

  lv_obj_t* container = nullptr;
  lib7842::OdomController* controller = nullptr;

  lv_style_t containerStyle;
    lv_style_t style_label;

  using Tuner = std::tuple<lv_obj_t*, std::vector<lv_obj_t*>, IterativePosPIDController*>;
  std::vector<Tuner> tuners = {};
  using Passer = std::tuple<ChassisTuner*, lv_obj_t*>;

  ChassisTuner(lv_obj_t*, lib7842::OdomController*);
  ChassisTuner(lv_obj_t*, lv_color_t, lib7842::OdomController*);
  ~ChassisTuner();

  Tuner buildTuner(lv_obj_t*, std::string, IterativePosPIDController*);

  static lv_res_t btnAction(lv_obj_t*, const char*);

};
