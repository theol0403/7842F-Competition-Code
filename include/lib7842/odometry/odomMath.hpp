#pragma once
#include "main.h"

namespace lib7842
{
  struct qPoint
  {
    QLength x {0_in};
    QLength y {0_in};
    QAngle theta {0_rad};
  };

  struct dPoint
  {
    double x = 0;
    double y = 0;
    double theta = 0;
  };
}

namespace lib7842::OdomMath
{
  dPoint toDPoint(qPoint);
  qPoint toQPoint(dPoint);

  dPoint add(dPoint, dPoint);
  dPoint sub(dPoint, dPoint);
  dPoint mult(dPoint, dPoint);
  dPoint div(dPoint, dPoint);
  double mag(dPoint);
  dPoint normalize(dPoint);
  dPoint multScalar(dPoint, double);
  double dot(dPoint, dPoint);

  dPoint closest(dPoint, dPoint, dPoint);
  qPoint closest(qPoint, qPoint);

  QAngle rollAngle360(QAngle);
  QAngle rollAngle180(QAngle);
  bool shouldDriveBackwards(QAngle);

  QLength computeDistanceBetweenPoints(qPoint, qPoint);
}
