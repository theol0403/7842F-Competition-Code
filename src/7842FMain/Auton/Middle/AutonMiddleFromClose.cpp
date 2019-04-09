#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonMiddleFromClose(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 7_ft);

  chassis.driveToPoint({3_ft, 8_ft}, 1.5, makeSettle(2_in)); // Move to shooting position

  chassis.turnToPoint(middleFlagShoot); // turn to flag
  
  waitForLastMinute(timer);
  robot.shooter->setDistanceToFlag(distanceToPoint(middleFlagShoot));
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

}
