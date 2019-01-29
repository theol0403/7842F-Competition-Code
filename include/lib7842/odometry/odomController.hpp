#pragma once
#include "main.h"
#include "odomTracker.hpp"
#include "odomMath.hpp"
#include "lib7842/pid/pidSystem.hpp"

namespace lib7842
{

  using namespace OdomMath;

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

    QAngle computeAngleOfPoint(Point);
    QAngle computeAngleToPoint(Point);
    QLength computeDistanceToPoint(Point);

    void turnToAngleSettle(QAngle, std::function<bool(OdomController*)> = turnSettle);
    void turnToAngle(QAngle, bool = true);
    void turnToPointSettle(Point, std::function<bool(OdomController*)> = turnSettle);
    void turnToPoint(Point, bool = true);
    void turnAngle(QAngle, bool = true);

    void driveDistanceAtAngleSettle(QLength, QAngle, std::function<bool(OdomController*)> = driveSettle);
    void driveDistanceAtAngle(QLength, QAngle, bool = true);
    void driveDistance(QLength, bool = true);

    void driveToPointSettle(Point, std::function<bool(OdomController*)> = driveSettle);
    void driveToPoint(Point, bool = true);
    void driveToPointAndAngle(Point);

  };

}
