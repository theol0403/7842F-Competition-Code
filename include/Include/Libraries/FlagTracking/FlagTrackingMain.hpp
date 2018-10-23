#include "main.h"
#pragma once



void mainFlagTrackingTask(void*);






struct flagSig_t
{
  int blueSig;
  int redSig;
};




struct simpleObjects
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





class VisionReading
{

private:

  pros::Vision m_thisVision;

  const int m_objectNum;

  int m_currentCount = 0;

  simpleObjects * m_flagObjects = nullptr;


public:


  VisionReading(int, int);
  ~VisionReading();

  int getObjects();

  int filterNoise(float minSize = 25);
  int filterObjectSize(float sizeThreshold = 0.5);
  int filterObjectProp(float propThreshold = 0.3, float wantedProp = 1.3); //Width:Height
  int discardObjects();

  simpleObjects* exportObjects();
  void debugObjects(int);
  void debugErrorSig();

};
