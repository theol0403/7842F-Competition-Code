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
    bool shouldDriveBackwards(QAngle);

    QAngle computeAngleOfPoint(Point);
    QAngle computeAngleToPoint(Point);

    QLength computeDistanceBetweenPoints(Point, Point);
    QLength computeDistanceToPoint(Point);
    
    static bool turnSettle(OdomController*);
    static bool turnNoSettle(OdomController*);
    static bool driveSettle(OdomController*);
    static bool driveNoSettle(OdomController*);

    void turnToAngle(QAngle, std::function<bool(OdomController*)>);
    void turnToAngle(QAngle, bool = true);
    void turnToPoint(Point, std::function<bool(OdomController*)>);
    void turnToPoint(Point, bool = true);
    void turnAngle(QAngle, bool = true);

    void driveDistanceAtAngle(QLength, QAngle, std::function<bool(OdomController*)>);
    void driveDistanceAtAngle(QLength, QAngle, bool = true);
    void driveDistance(QLength, bool = true);

    void driveToPoint(Point);
    void driveToPointAndAngle(Point);

  };

}
