#include "7842FMain/AutonIncludes.hpp"

void AutonFar_CapCrossPark(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 3_ft);

  robot.arm->setState(ArmController::balance);

  chassis.driveToPoint({3_ft, 3_ft}, 1, makeSettle(2_in));

  robot.intake->setState(IntakeController::off);
  robot.arm->setState(ArmController::down);

  chassis.turnAngle(mirrorSide(angleToPoint(farFlatCap), chassis.side) + 180_deg, pointTurn);
  chassis.driveDistance(-2_ft);
  robot.arm->setState(ArmController::carry);
  pros::delay(500);

  chassis.driveToPoint({2.7_ft, 1_ft});
  chassis.turnToPoint(farPost, capTurn);
  chassis.driveForTime(800, 0.7);

  robot.arm->setState(ArmController::up);
  pros::delay(800);
  robot.arm->setState(ArmController::off);
  chassis.driveDistance(-1_ft);
  robot.arm->setState(ArmController::down);

  chassis.turnToPoint(farFlagPost); // turn to flag
  robot.shooter->setDistanceToFlag(9_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTop);
  pros::delay(1000);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootMiddle);



}
