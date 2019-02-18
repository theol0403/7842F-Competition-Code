#pragma once
#include "main.h"
#include "lib7842/odometry/odomMath.hpp"
#include "lib7842/odometry/tracker.hpp"
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

    settleFunc_t makeSettle(QAngle);
    settleFunc_t makeSettle(QLength);
    settleFunc_t makeSettle(QLength, QAngle);
    bool turnSettle(OdomController*);
    bool driveSettle(OdomController*);

    turnFunc_t makeArc(double, double);
    void pointTurn(OdomController*, double);
    void leftPivot(OdomController*, double);
    void rightPivot(OdomController*, double);

    angleCalc_t angleCalc(QAngle);
    angleCalc_t angleCalc(qPoint);

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


  }
}
