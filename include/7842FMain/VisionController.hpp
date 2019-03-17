#pragma once
#include "main.h"
#include "lib7842/vision/visionReader.hpp"

class VisionController
{

public:

  pros::Vision* sensor = nullptr;
  lib7842::VisionReader reader;
  pros::Task visionTask;

  VisionController(pros::Vision*);

  void run();
  static void task(void*);

};
