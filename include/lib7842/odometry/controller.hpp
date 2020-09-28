#pragma once
#include "asyncAction.hpp"
#include "lib7842/filter/avgFilter.hpp"
#include "lib7842/other/miscUtils.hpp"
#include "main.h"
#include "odomMath.hpp"
#include "tracker.hpp"

namespace lib7842 {
using namespace OdomMath;

class OdomController;
class AsyncAction;

using settleFunc_t = std::function<bool(OdomController*)>;
using turnFunc_t = std::function<void(OdomController*, double)>;
using angleCalc_t = std::function<QAngle(OdomController*)>;

class OdomController {

public:
  OdomTracker* tracker = nullptr;
  IterativePosPIDController* distancePid = nullptr;
  IterativePosPIDController* anglePid = nullptr;
  IterativePosPIDController* turnPid = nullptr;
  const double slewRate;
  pros::Task slewTask;

  const QLength pointRadius; // radius to point before slowing down and ignoring angle
  QAngle angleErr = 0_deg;
  QLength distanceErr = 0_in;

  Timer abortTimer;

  bool enableVector = false;
  double wantedVelL = 0;
  double wantedVelR = 0;
  double lastVelL = 0;
  double lastVelR = 0;

  OdomController(OdomTracker*, IterativePosPIDController*, IterativePosPIDController*,
                 IterativePosPIDController*, double = 1);

  void resetPid();
  bool checkAbort(double, QTime);
  bool checkEmergencyAbort();

  void driveVectorTask();
  static void driveVectorTaskFnc(void*);
  void driveVector(double, double, bool = false);
  void stop();

  void runActions(AsyncActionList);

  QAngle angleToPoint(QPoint);
  QLength distanceToPoint(QPoint);

  static settleFunc_t makeSettle(QAngle);
  static settleFunc_t makeSettle(QLength);
  static settleFunc_t makeSettle(QLength, QAngle);
  static bool turnSettle(OdomController*);
  static bool driveSettle(OdomController*);

  static turnFunc_t makeArc(double, double);
  static void pointTurn(OdomController*, double);
  static void leftPivot(OdomController*, double);
  static void rightPivot(OdomController*, double);
  static void capTurn(OdomController*, double);

  static angleCalc_t angleCalc(QAngle);
  static angleCalc_t angleCalc(QPoint);
  static angleCalc_t angleDisable();

  void turn(angleCalc_t, turnFunc_t = pointTurn, settleFunc_t = turnSettle, AsyncActionList = {});
  void turnToAngle(QAngle, turnFunc_t = pointTurn, settleFunc_t = turnSettle, AsyncActionList = {});
  void turnAngle(QAngle, turnFunc_t = pointTurn, settleFunc_t = turnSettle, AsyncActionList = {});
  void turnToPoint(QPoint, turnFunc_t = pointTurn, settleFunc_t = turnSettle, AsyncActionList = {});

  void driveDistanceAtAngle(QLength, angleCalc_t, double = 1, settleFunc_t = driveSettle,
                            AsyncActionList = {});
  void driveDistance(QLength, settleFunc_t = driveSettle, AsyncActionList = {});
  void driveForTime(int, double, AsyncActionList = {});
  void driveForTimeAtAngle(int, double, angleCalc_t, double = 1, AsyncActionList = {});
  void allignToAngle(QAngle, double, double);

  void driveToPoint(QPoint, double = 1, settleFunc_t = driveSettle, AsyncActionList = {});
  void driveToPoint2(QPoint, double = 1, settleFunc_t = driveSettle, AsyncActionList = {});

  void drivePath(Path, double = 1, settleFunc_t = driveSettle, settleFunc_t = driveSettle,
                 AsyncActionList = {});
  void drivePath2(Path, double = 1, settleFunc_t = driveSettle, settleFunc_t = driveSettle,
                  AsyncActionList = {});
};

} // namespace lib7842
