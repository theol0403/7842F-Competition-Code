#pragma once
#include "main.h"
#include "lib7842/vision/visionReader.hpp"
#include "lib7842/vision/objDrawer.hpp"

class VisionController
{

public:

  pros::Vision* sensor = nullptr;
  lib7842::VisionReader reader;
  lv_obj_t* parent = nullptr;
  pros::Task visionTask;

  VisionController(pros::Vision*, lv_obj_t*);

  void run();
  static void task(void*);

};
