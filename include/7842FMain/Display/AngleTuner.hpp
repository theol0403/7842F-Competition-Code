#pragma once
#include "../Controllers/ShootController.hpp"
#include "main.h"

class AngleTuner {

public:
  lv_obj_t* container = nullptr;
  ShootController*& angler;
  pros::Task task;

  std::array<lv_obj_t*, 3> angleLabels = {};

  using DialLine = std::tuple<std::array<lv_point_t, 2>, lv_obj_t*>;
  std::array<DialLine, 3> dialLines = {};
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
