#pragma once
#include "main.h"
#include "odomMath.hpp"
#include "tracker.hpp"
#include "lib7842/pid/pidSystem.hpp"
#include "lib7842/auton/autonSelector.hpp"

namespace lib7842
{
  using namespace OdomMath;

  class OdomController;
  typedef std::function<bool(OdomController*)> settleFunc_t;
  typedef std::function<void(OdomController*, double)> turnFunc_t;

  settleFunc_t makeSettle(QAngle);
  settleFunc_t makeSettle(QLength);
  settleFunc_t makeSettle(QLength, QAngle);
  bool turnSettle(OdomController*);
  bool driveSettle(OdomController*);

  turnFunc_t makeArc(double, double);
  void pointTurn(OdomController*, double);
  void leftPivot(OdomController*, double);
  void rightPivot(OdomController*, double);

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

    #define velFilterSize 15
    AverageFilter<velFilterSize> velFilter;


    QAngle computeAngleToPoint(qPoint);
    QLength computeDistanceToPoint(qPoint);

    void driveVector(double, double);

    void resetVelocity(double);
    void resetVelocityActual();
    void resetVelocityMax();
    double getActualVelocity();
    double filterVelocity();
    double getFilteredVelocity();
    void resetEmergencyAbort();
    bool emergencyAbort();

    void turnToAngle(QAngle, turnFunc_t = pointTurn, settleFunc_t = turnSettle);
    void turnAngle(QAngle, turnFunc_t = pointTurn, settleFunc_t = turnSettle);
    void turnToPoint(qPoint, turnFunc_t = pointTurn, settleFunc_t = turnSettle);

    void driveDistanceAtAngle(QLength, QAngle, double = 1, settleFunc_t = driveSettle);
    void driveDistance(QLength, settleFunc_t = driveSettle);
    void driveForTime(int, double);
    void driveForTimeAtAngle(int, double, QAngle, double = 1);
    void allignToAngle(QAngle, double, double);

    void driveToPoint(qPoint, double = 1, settleFunc_t = driveSettle);
    void driveToPointSimple(qPoint, double = 1, settleFunc_t = driveSettle);

    void drivePath(Path, double = 1, settleFunc_t = driveSettle, settleFunc_t = driveSettle);
    void drivePathSimple(Path, double = 1, settleFunc_t = driveSettle, settleFunc_t = driveSettle);

  };

}
