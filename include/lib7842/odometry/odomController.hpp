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

    static std::function<bool(OdomController*)> createTurnSettle(QAngle);
    static std::function<bool(OdomController*)> createDriveSettle(QLength);

    QAngle computeAngleOfPoint(qPoint);
    QAngle computeAngleToPoint(qPoint);
    QLength computeDistanceToPoint(qPoint);

    void normalizeDrive(double &, double &);

    void turnToAngleSettle(QAngle, std::function<bool(OdomController*)> = turnSettle);
    void turnToAngle(QAngle, bool = true);
    void turnAngle(QAngle, bool = true);
    void turnToPointSettle(qPoint, std::function<bool(OdomController*)> = turnSettle);
    void turnToPoint(qPoint, bool = true);

    void driveDistanceAtAngleSettle(QLength, QAngle, std::function<bool(OdomController*)> = driveSettle, double = 1, bool = true);
    void driveDistanceAtAngle(QLength, QAngle, bool = true, double = 1);
    void driveDistance(QLength, bool = true);

    void driveToPoint1Settle(qPoint, std::function<bool(OdomController*)> = driveSettle, double = 4);
    void driveToPoint1(qPoint, bool = true, double = 4);

    void driveToPoint2Settle(qPoint, std::function<bool(OdomController*)> = driveSettle, double = 4);
    void driveToPoint2(qPoint, bool = true, double = 4);

    void driveForTime(double, int);
    void driveForTimeAtAngle(double, QAngle, int, double = 1);

  };

}
