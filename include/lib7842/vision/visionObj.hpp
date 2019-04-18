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
    avgDim,
    centerX,
    centerY,
    fromMidX,
    fromMidY,
    absFromMidX,
    absFromMidY
  };

  struct visionObj
  {
    int sig = VISION_OBJECT_ERR_SIG;
    double x = 0;
    double y = 0;
    double width = 0;
    double height = 0;
    double area = 0;
    double avgDim = 0;
    double centerX = 0;
    double centerY = 0;
    double fromMidX = 0;
    double fromMidY = 0;
    double absFromMidX = 0;
    double absFromMidY = 0;

    const visionObj& operator+(const visionObj&);
    const visionObj& operator-(const visionObj&);
    const visionObj& operator*(const visionObj&);
    const visionObj& operator/(const visionObj&);

    const visionObj& operator+(const double&);
    const visionObj& operator-(const double&);
    const visionObj& operator*(const double&);
    const visionObj& operator/(const double&);

    double getAttr(objAttr) const;
    void print();

    visionObj() = default;
    visionObj(pros::vision_object);
  };

}
