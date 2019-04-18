#pragma once
#include "main.h"

namespace lib7842
{

  enum class objAttr
  {
    sig,
    x,
    y,
    width,
    height,
    area,
    centerX,
    centerY,
    toCenterX,
    toCenterY
  };

  struct visionObj
  {
    int sig = VISION_OBJECT_ERR_SIG;
    double x = 0;
    double y = 0;
    double width = 0;
    double height = 0;
    double area = 0;
    double centerX = 0;
    double centerY = 0;
    double toCenterX = 0;
    double toCenterY = 0;

    visionObj operator+(visionObj);
    visionObj operator-(visionObj);
    visionObj operator*(visionObj);
    visionObj operator/(visionObj);

    visionObj operator+(double);
    visionObj operator-(double);
    visionObj operator*(double);
    visionObj operator/(double);

    double getAttr(objAttr) const;
    void print();

    visionObj() = default;
    visionObj(pros::vision_object);
  };

}
