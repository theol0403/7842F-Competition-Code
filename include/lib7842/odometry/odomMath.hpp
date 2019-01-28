#pragma once
#include "main.h"
#include "odomTracker.hpp"

namespace lib7842
{

  struct dPoint
  {
    double x;
    double y;
  };

  class OdomMath
  {

  public:

    static dPoint toDPoint(Point);
    static Point toPoint(dPoint);

    static dPoint add(dPoint, dPoint);
    static dPoint sub(dPoint, dPoint);
    static dPoint mult(dPoint, dPoint);
    static dPoint div(dPoint, dPoint);
    static double mag(dPoint);
    static dPoint normalize(dPoint);
    static dPoint multScalar(dPoint, double);
    static double dot(dPoint, dPoint);


    static QAngle rollAngle360(QAngle);
    static QAngle rollAngle180(QAngle);
    static bool shouldDriveBackwards(QAngle);

    static QLength computeDistanceBetweenPoints(Point, Point);

    static Point calculateClosestPoint(Point, Point);

  };

}
