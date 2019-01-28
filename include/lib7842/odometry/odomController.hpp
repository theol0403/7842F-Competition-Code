#pragma once
#include "main.h"
#include "odomTracker.hpp"
#include "odomMath.hpp"
#include "lib7842/pid/pidSystem.hpp"


namespace lib7842
{

  using lib7842::OdomMath;

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

    QAngle computeAngleOfPoint(Point);
    QAngle computeAngleToPoint(Point);
    QLength computeDistanceToPoint(Point);

    static std::function<bool(OdomController*)> turnSettle;
    static std::function<bool(OdomController*)> turnNoSettle;
    static std::function<bool(OdomController*)> driveSettle;
    static std::function<bool(OdomController*)> driveNoSettle;

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
