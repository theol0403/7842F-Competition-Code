#pragma once
#include "main.h"

namespace lib7842
{

  enum class autonSides {
    blue,
    red
  };

  struct dPoint; //Forward declare

  struct QPoint
  {
    QLength x {0_in};
    QLength y {0_in};
    QAngle theta {0_rad};

    QPoint(const QPoint&); //Needs to be by reference to avoid enless loop

    QPoint(QLength, QLength, QAngle);
    QPoint(QLength, QLength);
    QPoint();

    QPoint(QPoint, QLength, QLength);
    QPoint(QPoint, QAngle);

    QPoint(dPoint);

    QPoint operator+(QPoint);
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

    dPoint(QPoint);

    dPoint operator+(dPoint);
  };

  struct Path
  {
    std::vector<QPoint> wayPoints;
    Path(QPoint);
    Path(std::initializer_list<QPoint>);
    void add(QPoint);
    void add(Path);
  };

  QPoint mirrorSide(QPoint, autonSides);
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
  QPoint closest(QPoint, QPoint);

  QAngle rollAngle360(QAngle);
  QAngle rollAngle180(QAngle);
  QAngle rollAngle90(QAngle);

  QLength distanceBetweenPoints(QPoint, QPoint);
  QPoint distanceAtHeading(QLength, QAngle);

}
