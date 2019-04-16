#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonClose(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 7_ft);

  chassis.driveToPoint({0.9_ft, 7_ft}, 2); // Move to shooting position

  chassis.turnToPoint(sideFlagShoot); // turn to flag
  robot.shooter->setDistanceToFlag(7_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

  // Move forward towards flags and push bottom flag
  chassis.driveToPoint({0.9_ft, 10.8_ft}, 3, makeSettle(4_in));

  chassis.driveToPoint({3.7_ft, 8_ft}, 2, makeSettle(4_in));
  chassis.turnToAngle(0_deg);
  chassis.driveToPoint({3.7_ft, 7_ft}, 1, makeSettle(4_in));
  robot.arm->setState(ArmController::balance);
  chassis.driveForTime(1500, -1);
  chassis.turnAngle(20_deg);

  waitForLastMinute(timer);
  robot.shooter->setDistanceToFlag(5.5_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

}
