#pragma once
#include "objContainer.hpp"

namespace lib7842
{
  class VisionReader : public ObjContainer
  {

  private:

    pros::Vision* vision = nullptr;

    const int maxCount;
    std::vector<pros::vision_object> temp = {};

  public:

    VisionReader(pros::Vision*);

    VisionReader &getAll();
    VisionReader &getSig(int);
    VisionReader &getSig(std::initializer_list<int>);

    uint16_t createCode(int, int);

  };
}
