#pragma once
#include "objectTracking.hpp"

namespace lib7842
{

  class ObjectReading
  {

  private:

    pros::Vision* m_thisVision = nullptr;

  public:

    ObjectReading(pros::Vision&);
    ~ObjectReading();

    void getAllObjects(lib7842::ObjectContainer&);


  };
}
