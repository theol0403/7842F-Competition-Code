#pragma once
#include "lib7842/odometry/asyncAction.hpp"
#include "lib7842/odometry/controller.hpp"
#include "lib7842/odometry/odomMath.hpp"
#include "main.h"

namespace lib7842 {

class SideController {

public:
  OdomController* controller = nullptr;
  autonSides side = autonSides::red;

  SideController(OdomController*, autonSides);

  AsyncAction makeAsyncAction();

  void setState(QPoint);
  void setX(QLength);

  QAngle angleToPoint(QPoint);
  QLength distanceToPoint(QPoint);

  settleFunc_t makeSettle(QAngle);
  settleFunc_t makeSettle(QLength);
  settleFunc_t makeSettle(QLength, QAngle);

  turnFunc_t makeArc(double, double);
  void leftPivot(OdomController*, double);
  void rightPivot(OdomController*, double);

  angleCalc_t angleCalc(QAngle);
  angleCalc_t angleCalc(QPoint);

  void turn(angleCalc_t, turnFunc_t = OdomController::pointTurn,
            settleFunc_t = OdomController::turnSettle, AsyncActionList = {});
  void turnToAngle(QAngle, turnFunc_t = OdomController::pointTurn,
                   settleFunc_t = OdomController::turnSettle, AsyncActionList = {});
  void turnAngle(QAngle, turnFunc_t = OdomController::pointTurn,
                 settleFunc_t = OdomController::turnSettle, AsyncActionList = {});
  void turnToPoint(QPoint, turnFunc_t = OdomController::pointTurn,
                   settleFunc_t = OdomController::turnSettle, AsyncActionList = {});

  void driveDistanceAtAngle(QLength, angleCalc_t, double = 1,
                            settleFunc_t = OdomController::driveSettle, AsyncActionList = {});
  void driveDistance(QLength, settleFunc_t = OdomController::driveSettle, AsyncActionList = {});
  void driveForTime(int, double, AsyncActionList = {});
  void driveForTimeAtAngle(int, double, angleCalc_t, double = 1, AsyncActionList = {});
  void allignToAngle(QAngle, double, double);

  void driveToPoint(QPoint, double = 1, settleFunc_t = OdomController::driveSettle,
                    AsyncActionList = {});
  void driveToPoint2(QPoint, double = 1, settleFunc_t = OdomController::driveSettle,
                     AsyncActionList = {});

  void drivePath(Path, double = 1, settleFunc_t = OdomController::driveSettle,
                 settleFunc_t = OdomController::driveSettle, AsyncActionList = {});
  void drivePath2(Path, double = 1, settleFunc_t = OdomController::driveSettle,
                  settleFunc_t = OdomController::driveSettle, AsyncActionList = {});

  void driveToPointSimple(QPoint, settleFunc_t = OdomController::driveSettle, AsyncActionList = {});

#ifdef INSERT_CONTROLLER_NAME
  #define angleToPoint INSERT_CONTROLLER_NAME angleToPoint
  #define distanceToPoint INSERT_CONTROLLER_NAME distanceToPoint

  #define makeSettle INSERT_CONTROLLER_NAME makeSettle
  #define turnSettle OdomController::turnSettle
  #define driveSettle OdomController::driveSettle

  #define makeArc INSERT_CONTROLLER_NAME makeArc
  #define pointTurn OdomController::pointTurn
  #define leftPivot OdomController::leftPivot
  #define rightPivot OdomController::rightPivot
  #define capTurn OdomController::capTurn

  #define angleCalc INSERT_CONTROLLER_NAME angleCalc

//#define makeAsyncAction INSERT_CONTROLLER_NAME makeAsyncAction
#endif
};
} // namespace lib7842
