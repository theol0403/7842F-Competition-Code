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
  settleFunc_t makeSettle(QAngle);
  settleFunc_t makeSettle(QLength);
  settleFunc_t makeSettle(QLength, QAngle);
  bool turnSettle(OdomController*);
  bool driveSettle(OdomController*);

  typedef std::function<void(OdomController*, double)> turnFunc_t;
  turnFunc_t makeArc(double, double);
  void pointTurn(OdomController*, double);
  void leftPivot(OdomController*, double);
  void rightPivot(OdomController*, double);

  typedef std::function<bool(OdomController*)> triggerFunc_t;
  typedef std::function<void(OdomController*)> actionFunc_t;
  struct triggerAction_t
  {
    triggerFunc_t trigger;
    actionFunc_t action;
    bool triggered = false;
  };
  triggerFunc_t makeTrigger(qPoint, QLength);
  triggerFunc_t makeTrigger(qPoint, QLength, QAngle);
  triggerFunc_t makeTrigger(qPoint, QAngle);
  triggerFunc_t makeTrigger(QAngle);
  #define makeAction(x) [=](OdomController* that){ x }

  class OdomController
  {

  public:

    OdomController(
      OdomTracker*,
      PID *,
      PID *,
      PID *
    );

    OdomTracker *tracker = nullptr;

    PID *distancePid = nullptr;
    PID *anglePid = nullptr;
    PID *turnPid = nullptr;

    QAngle m_angleErr = 0_deg;
    QLength m_distanceErr = 0_in;

    #define velFilterSize 20
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
