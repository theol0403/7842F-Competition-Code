#pragma once
#include "main.h"
#include "odomMath.hpp"
#include "tracker.hpp"
#include "asyncAction.hpp"
#include "lib7842/pid/pidSystem.hpp"
#include "lib7842/auton/autonSelector.hpp"

namespace lib7842
{
  using namespace OdomMath;

  class OdomController;
  class AsyncAction;

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

  typedef std::function<QAngle(OdomController*)> angleCalc_t;
  angleCalc_t angleCalc(QAngle);
  angleCalc_t angleCalc(qPoint);

  using AsyncActionRef = std::reference_wrapper<AsyncAction>;
  using AsyncActionList = std::vector<AsyncActionRef>;

  class OdomController
  {

  public:

    OdomController(OdomTracker*, PID*, PID*, PID*);

    OdomTracker *tracker = nullptr;

    PID *distancePid = nullptr;
    PID *anglePid = nullptr;
    PID *turnPid = nullptr;

    QAngle m_angleErr = 0_deg;
    QLength m_distanceErr = 0_in;

    #define velFilterSize 20
    AverageFilter<velFilterSize> m_velFilter;


    void resetVelocity(double);
    void resetVelocityActual();
    void resetVelocityMax();
    double getActualVelocity();
    double filterVelocity();
    double getFilteredVelocity();
    void resetEmergencyAbort();
    bool emergencyAbort();

    void driveVector(double, double);
    void runActions(AsyncActionList);

    QAngle computeAngleToPoint(qPoint);
    QLength computeDistanceToPoint(qPoint);

    void turn(angleCalc_t, turnFunc_t = pointTurn, settleFunc_t = turnSettle, AsyncActionList = {});
    void turnToAngle(QAngle, turnFunc_t = pointTurn, settleFunc_t = turnSettle, AsyncActionList = {});
    void turnAngle(QAngle, turnFunc_t = pointTurn, settleFunc_t = turnSettle, AsyncActionList = {});
    void turnToPoint(qPoint, turnFunc_t = pointTurn, settleFunc_t = turnSettle, AsyncActionList = {});

    void driveDistanceAtAngle(QLength, angleCalc_t, double = 3, settleFunc_t = driveSettle, AsyncActionList = {});
    void driveDistance(QLength, settleFunc_t = driveSettle, AsyncActionList = {});
    void driveForTime(int, double, AsyncActionList = {});
    void driveForTimeAtAngle(int, double, angleCalc_t, double = 3, AsyncActionList = {});
    void allignToAngle(QAngle, double, double);

    void driveToPoint(qPoint, double = 3, settleFunc_t = driveSettle, AsyncActionList = {});
    void driveToPointSimple(qPoint, double = 3, settleFunc_t = driveSettle, AsyncActionList = {});

    void drivePath(Path, double = 3, settleFunc_t = driveSettle, settleFunc_t = driveSettle, AsyncActionList = {});
    void drivePathSimple(Path, double = 3, settleFunc_t = driveSettle, settleFunc_t = driveSettle, AsyncActionList = {});

  };

}
