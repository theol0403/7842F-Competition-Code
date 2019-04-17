#include "7842FMain/AutonIncludes.hpp"

void AutonFar_NearPark(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 3_ft);

  chassis.driveToPoint({1_ft, 3_ft}); // Move to shooting position

  chassis.turnToPoint(sideFlagShoot); // turn to flag

  robot.shooter->setDistanceToFlag(distanceToPoint(sideFlagShoot));
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

  //drive to platform
  chassis.driveToPoint({0.8_ft, 5_ft}, 1, makeSettle(2_in));
  chassis.turnToAngle(-90_deg);

  //drive up
  robot.arm->setState(ArmController::balance);
  chassis.driveForTime(2000, -1);
  chassis.driveDistance(0_in);

  //shoot middle flags from platform
  chassis.turnAngle(90_deg);
  waitForLastMinute(timer);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBothPlatform);
  
}
