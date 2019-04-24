#include "7842FMain/AutonIncludes.hpp"

void AutonFar_CapMiddle(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 2.9_ft);

  //back up and unlock arm
  robot.arm->setState(ArmController::balance);
  chassis.driveToPoint({3_ft, 2_ft}, 3, makeSettle(2_in));
  robot.arm->setState(ArmController::down);

  //get cap
  chassis.turnAngle(mirrorSide(angleToPoint(farFlatCap), chassis.side) + 180_deg);
  chassis.driveDistance(-2_ft, makeSettle(2_in));
  robot.arm->setState(ArmController::carry);
  pros::delay(300);

  //drive to post
  chassis.driveToPoint({2.5_ft, 1.45_ft}, 2, makeSettle(3_in));
  chassis.turnToPoint(farPost, capTurn);

  robot.arm->setState(ArmController::up);
  chassis.driveForTime(500, 0.6);
  pros::delay(200);
  robot.arm->setState(ArmController::off);

  chassis.driveToPoint({1_ft, 3_ft}, 1, makeSettle(3_in)); // Move to shooting position
  robot.arm->setState(ArmController::down);

  QPoint middleFlagShoot {6.5_ft, 11_ft};
  chassis.turnToPoint(middleFlagShoot); // turn to flag

  robot.intake->setState(IntakeController::intakeBall);

  robot.shooter->setDistanceToFlag(9_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTop);
  pros::delay(1000);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootMiddle);



}
