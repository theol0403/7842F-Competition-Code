#pragma once
#include "lib7842/vision/objContainer.hpp"
#include "lib7842/vision/objDrawer.hpp"
#include "lib7842/vision/visionObj.hpp"
#include "lib7842/vision/visionReader.hpp"
#include "main.h"

class VisionController {

public:
  pros::Vision* sensor = nullptr;
  lib7842::VisionReader reader;
  lib7842::ObjDrawer drawer;
  IterativePosPIDController* pid = nullptr;
  pros::Task task;

  lib7842::ObjContainer target;

  VisionController(pros::Vision*, lv_obj_t*, IterativePosPIDController*);

  double allign();

  void run();
  static void taskFnc(void*);
};
