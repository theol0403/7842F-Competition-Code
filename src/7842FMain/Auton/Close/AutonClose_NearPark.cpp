#include "7842FMain/AutonIncludes.hpp"

void AutonClose_NearPark(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 7_ft);

  chassis.driveToPoint({0.8_ft, 7_ft}, 2); // Move to shooting position

  chassis.turnToPoint(sideFlagShoot); // turn to flag
  robot.shooter->setDistanceToFlag(7_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

  // Move forward towards flags and push bottom flag
  chassis.driveToPoint({1_ft, 11_ft}, 3, makeSettle(4_in));

  //back up and drive to plaform
  if(chassis.side == autonSides::red) {
    chassis.driveToPoint({4.15_ft, 7.5_ft}, 2, makeSettle(4_in));
  } else {
    chassis.driveToPoint({3.8_ft, 7.5_ft}, 2, makeSettle(4_in));
  }

  chassis.turnToAngle(0_deg); //turn to platform

  //drive up
  robot.arm->setState(ArmController::balance);
  chassis.driveToPoint({3.65_ft, 7_ft}, 1, makeSettle(4_in));
  chassis.driveForTime(1800, -1);
  chassis.driveDistance(0_in);

  //shoot middle flags from platform
  chassis.turnAngle(15_deg);
  waitForLastMinute(timer);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBothPlatform);

}