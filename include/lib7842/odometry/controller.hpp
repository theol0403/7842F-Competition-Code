#pragma once
#include "main.h"
#include "odomMath.hpp"
#include "tracker.hpp"
#include "lib7842/pid/pidSystem.hpp"

namespace lib7842
{
  using namespace OdomMath;

  class OdomController
  {

  public:

    OdomTracker *chassis = nullptr;

    PID *distancePid = nullptr;
    PID *anglePid = nullptr;
    PID *turnPid = nullptr;

    QAngle m_angleErr = 0_deg;
    QLength m_distanceErr = 0_in;

    OdomController(
      OdomTracker*,
      PID *,
      PID *,
      PID *
    );

    static bool turnSettle(OdomController*);
    static bool driveSettle(OdomController*);

    static std::function<bool(OdomController*)> makeSettle(QAngle);
    static std::function<bool(OdomController*)> makeSettle(QLength);
    static std::function<bool(OdomController*)> makeSettle(QLength, QAngle);

    QAngle computeAngleToPoint(qPoint);
    QLength computeDistanceToPoint(qPoint);

    void normalizeDrive(double &, double &);

    void turnToAngle(QAngle, std::function<bool(OdomController*)> = turnSettle);
    void turnAngle(QAngle, std::function<bool(OdomController*)> = turnSettle);
    void turnToPoint(qPoint, std::function<bool(OdomController*)> = turnSettle);

    void driveDistanceAtAngle(QLength, QAngle, double = 1, std::function<bool(OdomController*)> = driveSettle, bool = true);
    void driveDistance(QLength, std::function<bool(OdomController*)> = driveSettle);

    void driveForTime(int, double);
    void driveForTimeAtAngle(int, double, QAngle, double = 1);

    void driveToPoint(qPoint, double = 4, std::function<bool(OdomController*)> = driveSettle);
    void driveToPointSimple(qPoint, double = 4, std::function<bool(OdomController*)> = driveSettle);

  };

}
