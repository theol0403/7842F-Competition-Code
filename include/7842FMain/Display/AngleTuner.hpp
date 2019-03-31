#pragma once
#include "main.h"
#include "../Controllers/ShootController.hpp"

class AngleTuner
{

public:

  lv_obj_t* container = nullptr;
  ShootController*& angler;
  pros::Task task;

  std::array<lv_obj_t*, 3> angleLabels = {};
  std::array<std::tuple<std::vector<lv_point_t>, lv_obj_t*>, 3> dialLines = {};
  lv_obj_t* dialLabel;

  AngleTuner(lv_obj_t*, ShootController*&);
  AngleTuner(lv_obj_t*, lv_color_t, ShootController*&);
  ~AngleTuner();

  void allignLabel(lv_obj_t*, std::string, double*);
  void calcAngleLabels();
  static lv_res_t angleBtnAction(lv_obj_t*, const char*);

  static lv_res_t actionBtnAction(lv_obj_t*, const char*);

  void calcDial();

  static void taskFnc(void*);

};
