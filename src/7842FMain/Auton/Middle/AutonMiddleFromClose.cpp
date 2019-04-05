#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonMiddleFromClose(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  chassis.setState({startX, 7_ft, 90_deg}); // Robot is facing cap

  AsyncAction intake = AsyncAction()
  .withTrigger(makeTrigger(return computeDistanceToPoint(closeCapDrive) < 2_ft;))
  .withMakeAction(robot.intake->setState(IntakeController::intakeBall););

  chassis.driveToPoint(closeCapDrive, 1, driveSettle, {intake}); // Move to ball under cap
  chassis.driveToPoint({3_ft, 8_ft}, 1.5, makeSettle(2_in)); // Move to shooting position

  while(timer.millis() < shootTime) pros::delay(20);

  chassis.turnToPoint(middleFlagShoot); // turn to flag
  robot.shooter->setDistanceToFlag(computeDistanceToPoint(middleFlagShoot));
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

}
