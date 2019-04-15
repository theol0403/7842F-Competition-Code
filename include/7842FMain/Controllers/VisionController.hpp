#pragma once
#include "main.h"
#include "lib7842/vision/visionObj.hpp"
#include "lib7842/odometry/controller.hpp"


class VisionController
{

public:

  pros::Vision* sensor = nullptr;
  lv_obj_t* parent = nullptr;
  std::shared_ptr<okapi::SkidSteerModel> chassis;
  pros::Task task;

  lib7842::visionObj targetObj;

  VisionController(pros::Vision*, lv_obj_t*, std::shared_ptr<okapi::SkidSteerModel>);

  void allign();

  void run();
  static void taskFnc(void*);

};
