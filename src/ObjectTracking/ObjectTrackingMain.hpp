#pragma once
#ifndef TRACKING_EMULATOR
#include "main.h"
void mainObjectTrackingTask(void*);
#endif

#ifdef TRACKING_EMULATOR
#define VISION_OBJECT_ERR_SIG 255
#define VISION_FOV_WIDTH 316
#define VISION_FOV_HEIGHT 212
namespace pros {void delay(int);}

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "lvgl/lvgl.h"
#endif

#define NUM_SIGNATURES 10
#include <vector>
#include <array>


struct simpleObjects_t
{
  int objSig = VISION_OBJECT_ERR_SIG;
  double objX = 0;
  double objY = 0;
  double objWidth = 0;
  double objHeight = 0;
  double objSize = 0; // Avg of width and height
  double objCenterX = 0;
  double objCenterY = 0;
  bool discardObject = false;
};

class ObjectContainer;
class ObjectDrawing;

#ifndef TRACKING_EMULATOR
#include "VisionReading.hpp"
#endif

#include "ObjectSmoothing.hpp"
#include "ObjectDrawing.hpp"
#include "ObjectContainer.hpp"
//#include "ObjectRecorder.hpp"
