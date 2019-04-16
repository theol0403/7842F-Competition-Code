#include "7842FMain/AutonIncludes.hpp"

void AutonFar(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 3_ft);

  chassis.driveToPoint({1_ft, 3_ft}); // Move to shooting position

  chassis.turnToPoint(sideFlagShoot); // turn to flag

  robot.shooter->setDistanceToFlag(distanceToPoint(sideFlagShoot));
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);
}
