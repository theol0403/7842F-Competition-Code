#pragma once
#include "objectContainer.hpp"

namespace lib7842
{
  class ObjectReading : public ObjectContainer
  {

  private:

    const int maxCount;
    std::vector<pros::vision_object> temp = {};

  public:

    pros::Vision* vision = nullptr;

    ObjectReading(pros::Vision*);
    ObjectReading(pros::Vision&);
    ObjectReading(int);

    void getAll();
    void getSig(int);
    void getSig(std::initializer_list<int>);

  };
}
