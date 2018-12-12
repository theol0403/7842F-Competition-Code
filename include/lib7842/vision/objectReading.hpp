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


    void getObjects();


  };

}
