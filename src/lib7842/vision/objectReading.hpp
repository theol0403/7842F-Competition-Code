#pragma once
#include "objectTracking.hpp"

namespace lib7842
{

  class ObjectReading
  {

  private:

    pros::Vision* m_thisVision = nullptr;

    lib7842::ObjectContainer* m_destContainer = nullptr;

    pros::vision_object* m_visionArray = nullptr; //Temp array for vision objects

  public:


    ObjectReading(pros::Vision&, lib7842::ObjectContainer&);
    ~ObjectReading();

    void resetObject(int);

    int getObjects();

    int filterNoise(float minSize = 25);
    int filterSize(float sizeThreshold = 0.5);
    int filterProp(float propThreshold = 0.3, float wantedProp = 0.8); //Width:Height = 1:x
    int discardObjects();

    void debugObjects(int);
    void debugErrorSig();

  };

}
