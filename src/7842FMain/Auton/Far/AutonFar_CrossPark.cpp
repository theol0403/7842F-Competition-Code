#include "7842FMain/AutonIncludes.hpp"

void AutonFar_CrossPark(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 3_ft);

  pros::delay(4000);
  chassis.turnToPoint(farFlagShoot); // turn to flag
  pros::delay(1000);

  robot.shooter->setDistanceToFlag(9_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTop);
  pros::delay(2000);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootMiddle);

  robot.arm->setState(ArmController::balance);

  chassis.turnToAngle(-180_deg);
  chassis.driveForTime(2000, -1);
  chassis.driveDistance(0_in);
  robot.arm->setState(ArmController::off);


}
