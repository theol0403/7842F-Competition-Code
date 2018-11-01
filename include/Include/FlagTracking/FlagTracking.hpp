#include "main.h"
#pragma once


void mainFlagTrackingTask(void*);


struct flagSig_t
{
  int blueSig;
  int redSig;
};


struct simpleObjects_t
{
  int objSig;
  int objX;
  int objY;
  int objWidth;
  int objHeight;
  int objSize; // Avg of width and height
  int objCenterX;
  int objCenterY;
  bool discardObject;
};


struct screenObjects_t
{
  lv_obj_t** screenObjectsArray = nullptr;
  int objectCount = 0;
  lv_style_t blueStyle;
  lv_style_t redStyle;
};
