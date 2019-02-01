#pragma once
#include "main.h"
#include "odomMath.hpp"
#include "odomTracker.hpp"
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
    static bool turnNoSettle(OdomController*);
    static bool driveSettle(OdomController*);
    static bool driveNoSettle(OdomController*);

    static std::function<bool(OdomController*)> createTurnSettle(QAngle);
    static std::function<bool(OdomController*)> createDriveSettle(QLength);

    QAngle computeAngleToPoint(qPoint);
    QLength computeDistanceToPoint(qPoint);

    void normalizeDrive(double &, double &);

    void turnToAngleSettle(QAngle, std::function<bool(OdomController*)> = turnSettle);
    void turnToAngle(QAngle, bool = true);
    void turnAngle(QAngle, bool = true);
    void turnToPointSettle(qPoint, std::function<bool(OdomController*)> = turnSettle);
    void turnToPoint(qPoint, bool = true);

    void driveDistanceAtAngleSettle(QLength, QAngle, double = 1, std::function<bool(OdomController*)> = driveSettle, bool = true);
    void driveDistanceAtAngle(QLength, QAngle, double = 1, bool = true);
    void driveDistance(QLength, bool = true);

    void driveForTime(int, double);
    void driveForTimeAtAngle(int, double, QAngle, double = 1);

    void driveToPointSettle(qPoint, double = 4, std::function<bool(OdomController*)> = driveSettle);
    void driveToPoint(qPoint, double = 4, bool = true);

    void driveToPointSimpleSettle(qPoint, double = 4, std::function<bool(OdomController*)> = driveSettle);
    void driveToPointSimple(qPoint, double = 4, bool = true);

  };

}
