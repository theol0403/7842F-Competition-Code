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
