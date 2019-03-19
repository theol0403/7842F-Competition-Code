#pragma once
#include "objContainer.hpp"

namespace lib7842
{
  class VisionReader : public ObjContainer
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
