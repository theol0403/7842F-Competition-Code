#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonCloseMiddle(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 7_ft);

  chassis.driveToPoint({1_ft, 7.5_ft}, 1.5, makeSettle(2_in)); // Move to shooting position

  chassis.turnToPoint(sideFlagShoot); // turn to flag
  robot.shooter->setDistanceToFlag(7_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

  chassis.driveToPointSimple(closeFlatCap, makeSettle(3_in));
  chassis.driveDistance(-4_in);
  pros::delay(2000);

  chassis.turnToPoint(middleFlagPost);

  waitForLastMinute(timer);
  robot.shooter->setDistanceToFlag(distanceToPoint(middleFlagShoot));
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

}
