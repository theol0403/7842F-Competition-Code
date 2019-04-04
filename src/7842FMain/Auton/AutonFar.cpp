#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonFar(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
auto &[chassis, timer] = *passer;

  chassis.setState({1_ft, 3_ft, 90_deg}); // Robot is facing cap

  AsyncAction intake = AsyncAction()
  .withTrigger(makeTrigger(return computeDistanceToPoint(farCapDrive) < 2_ft;))
  .withMakeAction(robot.intake->setState(IntakeController::intakeBall););

  chassis.driveToPoint(farCapDrive, 1, makeSettle(3_in), {intake}); // Move to ball under cap
  chassis.driveToPoint({1_ft, 3_ft}, 1, makeSettle(2_in)); // Move to behind shooting position

  chassis.turnToPoint(sideFlagShoot); // turn to flag

  robot.shooter->setDistanceToFlag(computeDistanceToPoint(sideFlagShoot));
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTop);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootMiddle);

}
