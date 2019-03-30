#pragma once
#include "main.h"

class AngleTuner
{

public:

  lv_obj_t* container = nullptr;
  std::array<lv_obj_t*, 3> angleLabels = {};

  pros::Task task;

  AngleTuner(lv_obj_t*);
  AngleTuner(lv_obj_t*, lv_color_t);
  ~AngleTuner();

  void calcAngleLabels();
  static lv_res_t angleBtnAction(lv_obj_t*, const char*);

  static lv_res_t commandBtnAction(lv_obj_t*, const char*);

  static void taskFnc(void*);

};
