#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonMiddleFromFar(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  chassis.setState({startX, 3_ft, 90_deg}); // Robot is facing cap

  AsyncAction intake = AsyncAction()
  .withTrigger(makeTrigger(return computeDistanceToPoint({4_ft, 3_ft}) < 2_ft;))
  .withMakeAction(robot.intake->setState(IntakeController::intakeBall););

  chassis.driveToPoint(farCapDrive, 1, driveSettle, {intake}); // Move to ball under cap
  chassis.driveToPoint({1_ft, 3_ft}, 1); // Move to shooting position

  chassis.turnToPoint(middleFlagShoot); // turn to flag
  pros::delay(2000);

  while(timer.millis() < shootTime) pros::delay(20);

  robot.shooter->setDistanceToFlag(computeDistanceToPoint(middleFlagShoot));
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

}
