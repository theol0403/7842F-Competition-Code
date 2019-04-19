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

    struct colorCode {
      pros::vision_color_code_t code = 0;
      int sig = 0;
    };

    VisionReader(pros::Vision*);

    VisionReader &getAll();
    VisionReader &getSig(int);
    VisionReader &getSig(std::initializer_list<int>);
    VisionReader &getCode(colorCode);
    VisionReader &getCode(std::initializer_list<colorCode>);

    colorCode createCode(int, int);

  };
}
