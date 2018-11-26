#pragma once
#include "ObjectTrackingMain.hpp"

class VisionReading
{

private:

  const pros::Vision* m_thisVision = nullptr;

  const int m_arrayLength;
  const ObjectContainer* m_destContainer = nullptr;

  int m_currentCount = 0;

  pros::vision_object* m_visionArray = nullptr; //Temp array for vision objects

public:


  VisionReading(int, int);
  ~VisionReading();

  void resetObject(int);

  int getObjects();

  int filterNoise(float minSize = 25);
  int filterSize(float sizeThreshold = 0.5);
  int filterProp(float propThreshold = 0.3, float wantedProp = 0.8); //Width:Height = 1:x
  int discardObjects();

  void exportObjects(objectContainer_t*);
  int exportCurrentCount();
  void debugObjects(int);
  void debugErrorSig();

};
