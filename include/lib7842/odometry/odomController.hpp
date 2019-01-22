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


    QAngle computeAngleToPoint(Point);
    void turnAngle(QAngle);
    void turnToAngle(QAngle);
    void turnToPoint(Point);

    QLength computeDistanceBetweenPoints(Point, Point);
    QLength computeDistanceToPoint(Point);

    void driveDistance(QLength);
    void driveDistanceToAngle(QLength, QAngle);

    void driveToPoint(Point);
    void driveToPointAndAngle(Point);


  };

}
