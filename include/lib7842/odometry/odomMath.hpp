#pragma once
#include "main.h"
#include "odomTracker.hpp"

namespace lib7842::OdomMath
{

  struct dPoint
  {
    double x;
    double y;
  };


  dPoint toDPoint(Point);
  Point toPoint(dPoint);

  dPoint add(dPoint, dPoint);
  dPoint sub(dPoint, dPoint);
  dPoint mult(dPoint, dPoint);
  dPoint div(dPoint, dPoint);
  double mag(dPoint);
  dPoint normalize(dPoint);
  dPoint multScalar(dPoint, double);
  double dot(dPoint, dPoint);

  dPoint closest(dPoint, dPoint, dPoint);
  Point closest(Point, Point);


  QAngle rollAngle360(QAngle);
  QAngle rollAngle180(QAngle);
  bool shouldDriveBackwards(QAngle);

  QLength computeDistanceBetweenPoints(Point, Point);


}
