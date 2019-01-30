#pragma once
#include "main.h"
#include "odomMath.hpp"
#include "odomTracker.hpp"
#include "lib7842/pid/pidSystem.hpp"

namespace lib7842
{

  class OdomController
  {

  public:

    OdomTracker *chassis;

    PID *distancePid;
    PID *anglePid;
    PID *turnPid;

    OdomController(
      OdomTracker*,
      PID *,
      PID *,
      PID *
    );

    static bool turnSettle(OdomController*);
    static bool turnNoSettle(OdomController*);
    static bool driveSettle(OdomController*);
    static bool driveNoSettle(OdomController*);

    static std::function<bool(OdomController*)> createTurnSettle(double);
    static std::function<bool(OdomController*)> createDriveSettle(double);

    QAngle computeAngleOfPoint(qPoint);
    QAngle computeAngleToPoint(qPoint);
    QLength computeDistanceToPoint(qPoint);

    void turnToAngleSettle(QAngle, std::function<bool(OdomController*)> = turnSettle);
    void turnToAngle(QAngle, bool = true);
    void turnToPointSettle(qPoint, std::function<bool(OdomController*)> = turnSettle);
    void turnToPoint(qPoint, bool = true);
    void turnAngle(QAngle, bool = true);

    void driveDistanceAtAngleSettle(QLength, QAngle, std::function<bool(OdomController*)> = driveSettle);
    void driveDistanceAtAngle(QLength, QAngle, bool = true);
    void driveDistance(QLength, bool = true);

    void driveToPointSettle(qPoint, std::function<bool(OdomController*)> = driveSettle);
    void driveToPoint(qPoint, bool = true);
    void driveToPointAndAngle(qPoint);

    void driveForTime(double, int);
    void driveForTimeAtAngle(double, QAngle, int);

  };

}
