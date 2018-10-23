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

  pros::vision_object* m_visionArray = nullptr; //Temp array for vision objects
  simpleObjects* m_flagObjects = nullptr;


public:


  VisionReading(int, int);
  ~VisionReading();

  void resetObject(int);

  int getObjects();

  int filterNoise(float minSize = 25);
  int filterSize(float sizeThreshold = 0.5);
  int filterProp(float propThreshold = 0.3, float wantedProp = 0.8); //Width:Height = 1:x
  int discardObjects();

  simpleObjects* exportObjects();
  void debugObjects(int);
  void debugErrorSig();

};
