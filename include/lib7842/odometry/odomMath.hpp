#pragma once
#include "main.h"

namespace lib7842
{

  enum class autonSides {
    blue,
    red
  };

  struct dPoint; //Forward declare

  struct qPoint
  {
    QLength x {0_in};
    QLength y {0_in};
    QAngle theta {0_rad};

    qPoint(const qPoint&); //Needs to be by reference to avoid enless loop

    qPoint(QLength, QLength, QAngle);
    qPoint(QLength, QLength);
    qPoint(QAngle);
    qPoint();

    qPoint(qPoint, QLength, QLength);
    qPoint(qPoint, QAngle);

    qPoint(dPoint);

    qPoint operator+(qPoint);
  };

  struct dPoint
  {
    double x {0};
    double y {0};
    double theta {0};

    dPoint(const dPoint&); //Needs to be by reference to avoid enless loop

    dPoint(double, double, double);
    dPoint(double, double);
    dPoint();

    dPoint(dPoint, double, double);
    dPoint(dPoint, double);

    dPoint(qPoint);

    dPoint operator+(dPoint);
  };

  struct Path
  {
    std::vector<qPoint> wayPoints;
    Path(qPoint);
    Path(std::initializer_list<qPoint>);
    void add(qPoint);
    void add(Path);
  };

  qPoint mirrorSide(qPoint, autonSides);
  QAngle mirrorSide(QAngle, autonSides);
  Path mirrorSide(Path, autonSides);
  QLength mirrorSide(QLength, autonSides);

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
  QAngle rollAngle90(QAngle);

  QLength distanceBetweenPoints(qPoint, qPoint);
  qPoint distanceAtHeading(QLength, QAngle);
  
}
