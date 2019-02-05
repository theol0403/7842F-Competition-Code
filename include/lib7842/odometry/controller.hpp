#pragma once
#include "main.h"
#include "odomMath.hpp"
#include "tracker.hpp"
#include "lib7842/pid/pidSystem.hpp"

#define DEFAULT_POINT_SCALE 4
#define DEFAULT_PATH_SETTLE 2_in

namespace lib7842
{
  using namespace OdomMath;

  class OdomController;
  typedef std::function<bool(OdomController*)> settleFunc_t;

  settleFunc_t makeSettle(QAngle);
  settleFunc_t makeSettle(QLength);
  settleFunc_t makeSettle(QLength, QAngle);

  class OdomController
  {

  public:

    OdomController(
      OdomTracker*,
      PID *,
      PID *,
      PID *
    );

    OdomTracker *chassis = nullptr;

    PID *distancePid = nullptr;
    PID *anglePid = nullptr;
    PID *turnPid = nullptr;

    QAngle m_angleErr = 0_deg;
    QLength m_distanceErr = 0_in;


    static bool turnSettle(OdomController*);
    static bool driveSettle(OdomController*);

    QAngle computeAngleToPoint(qPoint);
    QLength computeDistanceToPoint(qPoint);

    void driveVector(double, double);

    void turnToAngle(QAngle, settleFunc_t = turnSettle);
    void turnAngle(QAngle, settleFunc_t = turnSettle);
    void turnToPoint(qPoint, settleFunc_t = turnSettle);

    void driveDistanceAtAngle(QLength, QAngle, double = 1, settleFunc_t = driveSettle);
    void driveDistance(QLength, settleFunc_t = driveSettle);

    void driveForTime(int, double);
    void driveForTimeAtAngle(int, double, QAngle, double = 1);

    void driveToPoint(qPoint, double = DEFAULT_POINT_SCALE, settleFunc_t = driveSettle);
    void driveToPointSimple(qPoint, double = DEFAULT_POINT_SCALE, settleFunc_t = driveSettle);

    void drivePath(Path, double = DEFAULT_POINT_SCALE, settleFunc_t = driveSettle, settleFunc_t = driveSettle);
    void drivePathSimple(Path, double = DEFAULT_POINT_SCALE, settleFunc_t = driveSettle, settleFunc_t = driveSettle);

  };

}
