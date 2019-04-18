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

  private:

    int sig = VISION_OBJECT_ERR_SIG;
    double x = 0;
    double y = 0;
    double width = 0;
    double height = 0;

  public:

    const visionObj& operator+(const visionObj&);
    const visionObj& operator-(const visionObj&);
    const visionObj& operator*(const visionObj&);
    const visionObj& operator/(const visionObj&);

    const visionObj& operator+(const double&);
    const visionObj& operator-(const double&);
    const visionObj& operator*(const double&);
    const visionObj& operator/(const double&);

    double getAttr(objAttr) const;
    void setAttr(objAttr, double);

    void snapTo(objAttr, double);
    void snapToGrid(objAttr, int, double);
    void snapToGridX(int);
    void snapToGridY(int);

    void print();

    visionObj() = default;
    visionObj(pros::vision_object);

  };

}
