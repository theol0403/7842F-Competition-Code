#pragma once
#include "main.h"
#include "lib7842/odometry/controller.hpp"

class ChassisTuner
{

public:

  lv_obj_t* container = nullptr;
  lib7842::OdomController* controller = nullptr;

  lv_style_t containerStyle;

  std::vector<lv_obj_t*> tuners = {};
  using Passer = std::tuple<ChassisTuner*, IterativePosPIDController*>;

  ChassisTuner(lv_obj_t*, lib7842::OdomController*);
  ChassisTuner(lv_obj_t*, lv_color_t, lib7842::OdomController*);
  ~ChassisTuner();

  lv_obj_t* buildTuner(lv_obj_t*, std::string, IterativePosPIDController*);

  static lv_res_t btnAction(lv_obj_t*, const char*);

};
