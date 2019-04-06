#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonFar(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 3_ft);

  chassis.driveToPoint({1_ft, 3_ft}); // Move to shooting position

  chassis.turnToPoint({1.9_ft, 11.2_ft}); // turn to flag

  robot.shooter->setDistanceToFlag(8.5_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

}
