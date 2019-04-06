#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonCloseWithoutPush(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 7_ft);

  chassis.driveToPoint({1_ft, 7.5_ft}, 1.5, makeSettle(2_in)); // Move to shooting position

  chassis.turnToPoint(sideFlagShoot); // turn to flag
  robot.shooter->setDistanceToFlag(7_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

  chassis.driveToPoint({2_ft, 9_ft}, 1, makeSettle(2_in));

  chassis.turnToPoint(middleFlagShoot);

  waitForLastMinute(timer);
  robot.shooter->setDistanceToFlag(5.5_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);
}
