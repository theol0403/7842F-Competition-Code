#pragma once
#include "main.h"
#include "lib7842/vision/visionReader.hpp"
#include "lib7842/vision/objDrawer.hpp"
#include <sstream>
#include <iomanip>

class VisionController
{

public:

  pros::Vision* sensor = nullptr;
  lv_obj_t* parent = nullptr;

  pros::Task task;

  VisionController(pros::Vision*, lv_obj_t*);

  void run();
  static void taskFnc(void*);

};
