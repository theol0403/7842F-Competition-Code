#include "7842FMain/AutonIncludes.hpp"

void AutonClose(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 7_ft);

  chassis.driveToPoint({0.8_ft, 7_ft}, 2); // Move to shooting position

  chassis.turnToPoint(sideFlagShoot); // turn to flag
  robot.shooter->setDistanceToFlag(7_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

  // Move forward towards flags and push bottom flag
  chassis.driveToPoint({0.9_ft, 11_ft}, 3, makeSettle(4_in));

  chassis.driveToPoint({3.6_ft, 8_ft}, 2, makeSettle(4_in));
  chassis.turnToAngle(0_deg);
  robot.arm->setState(ArmController::balance);
  chassis.driveToPoint({3.6_ft, 7_ft}, 1, makeSettle(4_in));
  chassis.driveForTime(1900, -1);
  chassis.driveDistance(0_in);
  chassis.turnAngle(10_deg);

  waitForLastMinute(timer);
  robot.shooter->setTarget(15);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTarget);
  robot.shooter->setTarget(20);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTarget);

}
