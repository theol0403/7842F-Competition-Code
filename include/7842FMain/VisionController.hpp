#pragma once
#include "main.h"
#include "lib7842/vision/objectReading.hpp"

class VisionController
{

public:

  pros::Vision* sensor = nullptr;
  lib7842::ObjectReading vision;
  pros::Task visionTask;

  VisionController(pros::Vision*);

  void run();
  static void task(void*);

};
