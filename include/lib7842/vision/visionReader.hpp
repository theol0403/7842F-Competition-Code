#pragma once
#include "objectContainer.hpp"

namespace lib7842
{
  class VisionReader : public ObjectContainer
  {

  private:

    const int maxCount;
    std::vector<pros::vision_object> temp = {};

  public:

    pros::Vision* vision = nullptr;

    VisionReader(pros::Vision*);

    void getAll();
    void getSig(int);
    void getSig(std::initializer_list<int>);

  };
}
