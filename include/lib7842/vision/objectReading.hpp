#pragma once
#include "objectTracking.hpp"
#include "objectContainer.hpp"

namespace lib7842
{
  class ObjectReading : public ObjectContainer
  {

  private:

    pros::Vision vision;

  public:

    ObjectReading(int);

    void getAll();
    void getSig(std::initializer_list<int>);

  };
}
