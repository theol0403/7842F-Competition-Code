#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonFarDescore(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 3_ft);
  //chassis.driveDistance(5_in, makeSettle(4_in));

  //chassis.driveToPoint({0.9_ft, 3_ft}, 1, makeSettle(3_in)); // Move to shooting position

  pros::delay(4000);
  chassis.turnToPoint(farFlagShoot); // turn to flag
  pros::delay(1000);

  //waitForLastMinute(timer);

  robot.shooter->setDistanceToFlag(9_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTop);
  pros::delay(2000);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootMiddle);

  robot.arm->setState(ArmController::balance);

  chassis.turnToAngle(-180_deg);
  chassis.driveForTime(1500, -1);
  robot.arm->setState(ArmController::off);
  //chassis.driveDistance(-0.1_ft);


}
