#pragma once
#include "main.h"
#include "odomTracker.hpp"
#include "lib7842/pid/pidSystem.hpp"

namespace lib7842
{

  class OdomController
  {

  private:

    OdomTracker *chassis;

    PID *distancePid;
    PID *anglePid;
    PID *turnPid;

  public:

    OdomController(
      OdomTracker*,
      PID *,
      PID *,
      PID *
    );

    QAngle rollAngle360(QAngle);
    QAngle rollAngle180(QAngle);

    QAngle computeAngleOfPoint(Point);
    QAngle computeAngleToPoint(Point);

    void turnToAngle(QAngle);
    void turnToPoint(Point);
    void turnAngle(QAngle);

    QLength computeDistanceBetweenPoints(Point, Point);
    QLength computeDistanceToPoint(Point);

    void driveDistanceToAngle(QLength, QAngle);
    void driveDistanceToAngle(QLength, QAngle, std::function<bool(OdomController*)>);


    void driveDistance(QLength, bool = true);

    void driveToPoint(Point);
    void driveToPointAndAngle(Point);


  };

}
