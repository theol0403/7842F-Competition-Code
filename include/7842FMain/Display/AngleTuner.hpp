#pragma once
#include "main.h"
#include "../Controllers/ShootController.hpp"

class AngleTuner
{

public:

  lv_obj_t* container = nullptr;
  ShootController*& angler;
  std::array<lv_obj_t*, 3> angleLabels = {};

  pros::Task task;

  AngleTuner(lv_obj_t*, ShootController*&);
  AngleTuner(lv_obj_t*, lv_color_t, ShootController*&);
  ~AngleTuner();

  void allignLabel(lv_obj_t*, std::string, double*);
  void calcAngleLabels();
  static lv_res_t angleBtnAction(lv_obj_t*, const char*);

  static lv_res_t commandBtnAction(lv_obj_t*, const char*);

  static void taskFnc(void*);

};
