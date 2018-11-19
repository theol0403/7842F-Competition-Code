#pragma once
#include "main.h"
#include "FlagTrackingMain.hpp"

class VisionReading
{

private:

  pros::Vision m_thisVision;

  const int m_objectCount;
  int m_currentCount = 0;

  pros::vision_object* m_visionArray = nullptr; //Temp array for vision objects
  simpleObjects_t* m_flagObjects = nullptr;


public:


  VisionReading(int, int);
  ~VisionReading();

  void resetObject(int);

  int getObjects();

  int filterNoise(float minSize = 25);
  int filterSize(float sizeThreshold = 0.5);
  int filterProp(float propThreshold = 0.3, float wantedProp = 0.8); //Width:Height = 1:x
  int discardObjects();

  simpleObjects_t* exportObjects();
  int exportCurrentCount();
  void debugObjects(int);
  void debugErrorSig();

};
