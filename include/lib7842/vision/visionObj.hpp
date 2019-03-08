#pragma once
#include "objectTracking.hpp"

namespace lib7842
{

  enum class objAttr
  {
    objSig,
    objX,
    objY,
    objWidth,
    objHeight,
    objArea,
    objCenterX,
    objCenterY
  };

  struct visionObj
  {
    int objSig = VISION_OBJECT_ERR_SIG;
    double objX = 0;
    double objY = 0;
    double objWidth = 0;
    double objHeight = 0;
    double objArea = 0;
    double objCenterX = 0;
    double objCenterY = 0;

    visionObj operator+(visionObj);
    visionObj operator-(visionObj);
    visionObj operator*(visionObj);
    visionObj operator/(visionObj);

    visionObj operator+(double);
    visionObj operator-(double);
    visionObj operator*(double);
    visionObj operator/(double);

    double getAttr(objAttr) const;
  };

}
