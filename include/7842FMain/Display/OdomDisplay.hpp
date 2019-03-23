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

  lv_obj_t* led = nullptr;

  pros::Task task;


  OdomDisplay(lv_obj_t*, lib7842::OdomTracker*);
  OdomDisplay(lv_obj_t*, lv_color_t, lib7842::OdomTracker*);
  ~OdomDisplay();

  void drawRobot();

  static void taskFnc(void*);

};
