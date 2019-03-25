#pragma once
#include "main.h"
#include "lib7842/odometry/odomMath.hpp"
#include "lib7842/odometry/controller.hpp"
#include "lib7842/odometry/asyncAction.hpp"
#include "lib7842/auton/autonSelector.hpp"


namespace lib7842
{

  class SideController
  {

  public:

    OdomController *controller = nullptr;
    autonSides side = autonSides::red;

    SideController(OdomController*, autonSides);

    void setState(qPoint);
    void setX(QLength);

    QAngle computeAngleToPoint(qPoint);
    QLength computeDistanceToPoint(qPoint);

    settleFunc_t makeSettle(QAngle);
    settleFunc_t makeSettle(QLength);
    settleFunc_t makeSettle(QLength, QAngle);

    turnFunc_t makeArc(double, double);
    void leftPivot(OdomController*, double);
    void rightPivot(OdomController*, double);

    angleCalc_t angleCalc(QAngle);
    angleCalc_t angleCalc(qPoint);

    void turn(angleCalc_t, turnFunc_t = OdomController::pointTurn, settleFunc_t = OdomController::turnSettle, AsyncActionList = {});
    void turnToAngle(QAngle, turnFunc_t = OdomController::pointTurn, settleFunc_t = OdomController::turnSettle, AsyncActionList = {});
    void turnAngle(QAngle, turnFunc_t = OdomController::pointTurn, settleFunc_t = OdomController::turnSettle, AsyncActionList = {});
    void turnToPoint(qPoint, turnFunc_t = OdomController::pointTurn, settleFunc_t = OdomController::turnSettle, AsyncActionList = {});

    void driveDistanceAtAngle(QLength, angleCalc_t, double = 1, settleFunc_t = OdomController::driveSettle, AsyncActionList = {});
    void driveDistance(QLength, settleFunc_t = OdomController::driveSettle, AsyncActionList = {});
    void driveForTime(int, double, AsyncActionList = {});
    void driveForTimeAtAngle(int, double, angleCalc_t, double = 1, AsyncActionList = {});
    void allignToAngle(QAngle, double, double);

    void driveToPoint(qPoint, double = 1, settleFunc_t = OdomController::driveSettle, AsyncActionList = {});
    void driveToPoint2(qPoint, double = 1, settleFunc_t = OdomController::driveSettle, AsyncActionList = {});

    void drivePath(Path, double = 1, settleFunc_t = OdomController::driveSettle, settleFunc_t = OdomController::driveSettle, AsyncActionList = {});
    void drivePath2(Path, double = 1, settleFunc_t = OdomController::driveSettle, settleFunc_t = OdomController::driveSettle, AsyncActionList = {});

    void driveToPointSimple(qPoint, settleFunc_t = OdomController::driveSettle, AsyncActionList = {});


    #ifdef CONTROLLER_NAME_FOR_SHORTCUTS
    #define computeAngleToPoint CONTROLLER_NAME_FOR_SHORTCUTS computeAngleToPoint
    #define computeDistanceToPoint CONTROLLER_NAME_FOR_SHORTCUTS computeDistanceToPoint

    #define makeSettle CONTROLLER_NAME_FOR_SHORTCUTS makeSettle
    #define turnSettle OdomController::turnSettle
    #define driveSettle OdomController::driveSettle

    #define makeArc CONTROLLER_NAME_FOR_SHORTCUTS makeArc
    #define pointTurn OdomController::pointTurn
    #define leftPivot CONTROLLER_NAME_FOR_SHORTCUTS leftPivot
    #define rightPivot CONTROLLER_NAME_FOR_SHORTCUTS rightPivot

    #define angleCalc CONTROLLER_NAME_FOR_SHORTCUTS angleCalc
    #endif


  };
}
