#pragma once
#include "ObjectTrackingMain.hpp"

class ObjectContainer;

class VisionReading
{

private:

  pros::Vision* m_thisVision = nullptr;

  ObjectContainer* m_destContainer = nullptr;

  pros::vision_object* m_visionArray = nullptr; //Temp array for vision objects

public:


  VisionReading(pros::Vision&, ObjectContainer&);
  ~VisionReading();

  void resetObject(int);

  int getObjects();

  int filterNoise(float minSize = 25);
  int filterSize(float sizeThreshold = 0.5);
  int filterProp(float propThreshold = 0.3, float wantedProp = 0.8); //Width:Height = 1:x
  int discardObjects();

  void debugObjects(int);
  void debugErrorSig();

};
