#pragma once
#include "main.h"
#include "lib7842/vision/visionObj.hpp"
#include "lib7842/vision/objContainer.hpp"
#include "lib7842/odometry/controller.hpp"


class VisionController
{

public:

  pros::Vision* sensor = nullptr;
  lv_obj_t* parent = nullptr;
  std::shared_ptr<okapi::SkidSteerModel> chassis;
  pros::Task task;

  lib7842::ObjContainer target;

  VisionController(pros::Vision*, lv_obj_t*, std::shared_ptr<okapi::SkidSteerModel>);

  void allign(lib7842::autonSides);

  void run();
  static void taskFnc(void*);

};
