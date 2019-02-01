#pragma once
#include "main.h"

namespace lib7842
{
  struct dPoint; //Forward declare

  struct qPoint
  {
    QLength x {0_in};
    QLength y {0_in};
    QAngle theta {0_rad};

    qPoint(qPoint&); //Needs to be by reference to avoid enless loop

    qPoint(QLength, QLength, QAngle);
    qPoint(QLength, QLength);
    qPoint(QAngle);
    qPoint();

    qPoint(qPoint, QLength, QLength);
    qPoint(qPoint, QAngle);

    qPoint(dPoint);
  };

  struct dPoint
  {
    double x {0};
    double y {0};
    double theta {0};

    dPoint(dPoint&); //Needs to be by reference to avoid enless loop

    dPoint(double, double, double);
    dPoint(double, double);
    dPoint();

    dPoint(dPoint, double, double);
    dPoint(dPoint, double);

    dPoint(qPoint);
  };

  struct Path
  {
    std::vector<qPoint> wayPoints {};
    Path(qPoint);
    Path(std::initializer_list<qPoint>);
    void add(qPoint);
    void add(std::initializer_list<qPoint>);
  };

}

namespace lib7842::OdomMath
{
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
