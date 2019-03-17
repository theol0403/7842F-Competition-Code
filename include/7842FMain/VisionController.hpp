#pragma once
#include "main.h"

class VisionController
{

public:

  ObjectReading* vision;

  VisionController(ObjectReading*);

  void run();
  static void task(void*);

};
