#pragma once
#include "main.h"
#include "lib7842/vision/objectReading.hpp"

class VisionController
{

public:

  pros::Task visionTask;
  lib7842::ObjectReading* vision;

  VisionController(lib7842::ObjectReading*);

  void run();
  static void task(void*);

};
