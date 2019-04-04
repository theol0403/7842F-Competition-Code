#pragma once
#include "main.h"
#include "lib7842/odometry/tracker.hpp"

class OdomDisplay
{

public:

  lv_obj_t* container = nullptr;
  lib7842::OdomTracker* tracker = nullptr;

  lv_obj_t* field = nullptr;
  double fieldDim = 0;

  pros::Task task;


  OdomDisplay(lv_obj_t*, lib7842::OdomTracker*);
  OdomDisplay(lv_obj_t*, lv_color_t, lib7842::OdomTracker*);
  ~OdomDisplay();

  static lv_res_t tileAction(lv_obj_t*);
  static lv_res_t resetAction(lv_obj_t*);
  static lv_res_t btnmAction(lv_obj_t*);

  void run();
  static void taskFnc(void*);

};
