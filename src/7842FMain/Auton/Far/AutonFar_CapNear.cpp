#include "7842FMain/AutonIncludes.hpp"

void AutonFar_CapNear(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 3_ft);

  //back up and unlock arm
  robot.arm->setState(ArmController::balance);
  chassis.driveToPoint({3_ft, 3_ft}, 1, makeSettle(2_in));
  robot.arm->setState(ArmController::down);

  robot.intake->setState(IntakeController::off);

  //get cap
  chassis.turnAngle(mirrorSide(angleToPoint(farFlatCap), chassis.side) + 180_deg);
  chassis.driveDistance(-2_ft, makeSettle(2_in));
  robot.arm->setState(ArmController::carry);
  pros::delay(300);

  //drive to post
  chassis.driveToPoint({2.5_ft, 1.3_ft}, 2, makeSettle(3_in));
  chassis.turnToPoint(farPost, capTurn);

  robot.arm->setState(ArmController::up);
  chassis.driveForTime(500, 0.5);
  robot.arm->setState(ArmController::off);

  chassis.driveToPoint({1_ft, 3_ft}); // Move to shooting position
  robot.arm->setState(ArmController::down);

  chassis.turnToPoint(sideFlagShoot); // turn to flag
  robot.intake->setState(IntakeController::intakeBall);

  robot.shooter->setDistanceToFlag(distanceToPoint(sideFlagShoot));
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);



}
