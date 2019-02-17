#pragma once
#include "main.h"

#include "7842FMain/MotorConfig.hpp"
#include "lib7842/other/miscUtils.hpp"
#include "lib7842/auton/autonSelector.hpp"

#include "7842FMain/Shared/IntakeTask.hpp"
#include "7842FMain/Shared/FlywheelTask.hpp"

/**
* Points
*/
extern qPoint leftFlag;
extern qPoint rightFlag;

/**
* Functions
*/
void AutoNothing(autonSides);
void AutoTest(autonSides);
void AutoClose(autonSides);
void AutoFar(autonSides);
void AutoMiddle(autonSides);

/**
* Side Switcher
*/
// #define makeArc(x, y) (side == autonSides::red ? makeArc(x, y) : makeArc(y, x))
// #define leftPivot (side == autonSides::red ? leftPivot : rightPivot)
// #define rightPivot (side == autonSides::red ? rightPivot : leftPivot)
//
// #define angleCalc(x) angleCalc(setSide(x, side))
//
// #define computeAngleToPoint(x) computeAngleToPoint(setSide(x, side))
// #define computeDistanceToPoint(x) computeDistanceToPoint(setSide(x, side))
//
// #define turnToAngle(x, ...) turnToAngle(setSide(x, side), ##__VA_ARGS__)
// #define turnAngle(x, ...) turnAngle(setSide(x, side), ##__VA_ARGS__)
// #define turnToPoint(x, ...) turnToPoint(setSide(x, side), ##__VA_ARGS__)
//
// #define driveDistanceAtAngle(x, ...) driveDistanceAtAngle(setSide(x, side), ##__VA_ARGS__)
// #define driveDistance(x, ...) driveDistance(setSide(x, side), ##__VA_ARGS__)
// #define allignToAngle(x, ...) allignToAngle(setSide(x, side), ##__VA_ARGS__)
//
// // #define driveToPoint(x, ...) driveToPoint(setSide(x, side), ##__VA_ARGS__)
// #define driveToPointSimple(x, ...) driveToPointSimple(setSide(x, side), ##__VA_ARGS__)
//
// #define drivePath(x, ...) drivePath(setSide(x, side), ##__VA_ARGS__)
// #define drivePathSimple(x, ...) drivePathSimple(setSide(x, side), ##__VA_ARGS__)
