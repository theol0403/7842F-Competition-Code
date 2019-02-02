#pragma once
#include "main.h"
#include "odomMath.hpp"
#include "tracker.hpp"
#include "lib7842/pid/pidSystem.hpp"

namespace lib7842
{
  using namespace OdomMath;

  class OdomController;
  typedef std::function<bool(OdomController*)> settleFunction_t;

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

    static settleFunction_t makeSettle(QAngle);
    static settleFunction_t makeSettle(QLength);
    static settleFunction_t makeSettle(QLength, QAngle);

    QAngle computeAngleToPoint(qPoint);
    QLength computeDistanceToPoint(qPoint);

    void normalizeDrive(double &, double &);

    void turnToAngle(QAngle, settleFunction_t = turnSettle);
    void turnAngle(QAngle, settleFunction_t = turnSettle);
    void turnToPoint(qPoint, settleFunction_t = turnSettle);

    void driveDistanceAtAngle(QLength, QAngle, double = 1, settleFunction_t = driveSettle, bool = true);
    void driveDistance(QLength, settleFunction_t = driveSettle);

    void driveForTime(int, double);
    void driveForTimeAtAngle(int, double, QAngle, double = 1);

    void driveToPoint(qPoint, double = 4, settleFunction_t = driveSettle);
    void driveToPointSimple(qPoint, double = 4, settleFunction_t = driveSettle);

  };

}
