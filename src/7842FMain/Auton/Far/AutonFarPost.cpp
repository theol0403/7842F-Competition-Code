#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonFarPost(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  chassis.setState({5_in, 2_ft + 9.5_in, -63_deg}); // Robot is facing cap

  chassis.driveToPoint(farFlatCap, 0); // Move to shooting position
  robot.arm->setState(ArmController::carry);

  pros::delay(20000000);
  chassis.turnToAngle(angleToPoint(farFlatCap) - 180_deg);
  chassis.driveDistance(1_ft);



  chassis.turnToPoint(farFlagShoot); // turn to flag
  pros::delay(4000);

  //waitForLastMinute(timer);

  robot.shooter->setDistanceToFlag(9_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTop);
  pros::delay(2000);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootMiddle);

  robot.arm->setState(ArmController::balance);

  chassis.turnToAngle(-180_deg);
  chassis.driveForTime(100, 0.7);
  chassis.driveForTime(2400, -1);
  robot.arm->setState(ArmController::off);
  chassis.driveDistance(0.1_ft);


}
