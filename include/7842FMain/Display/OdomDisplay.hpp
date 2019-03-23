#pragma once
#include "main.h"
#include "lib7842/odometry/tracker.hpp"

class OdomDisplay
{

public:

  lv_obj_t* container = nullptr;

  pros::Task task;

  lv_obj_t* arrow = nullptr;

  OdomDisplay(lv_obj_t*, lib7842::OdomTracker*);
  OdomDisplay(lv_obj_t*, lv_color_t, lib7842::OdomTracker*);
  ~OdomDisplay();

  void run();
  static void taskFnc(void*);

};
