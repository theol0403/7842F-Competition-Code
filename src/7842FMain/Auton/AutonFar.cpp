#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonFar(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
auto &[chassis, timer] = *passer;

  chassis.setState({startX, 3_ft, 90_deg}); // Robot is facing cap

  AsyncAction intake = AsyncAction()
  .withTrigger(makeTrigger(return computeDistanceToPoint(farCapDrive) < 2_ft;))
  .withMakeAction(robot.intake->setState(IntakeController::intakeBall););

  chassis.driveToPoint(farCapDrive, 1, driveSettle, {intake}); // Move to ball under cap while intaking
  chassis.driveToPoint({1_ft, 3_ft}); // Move to shooting position

  chassis.turnToPoint(sideFlagShoot); // turn to flag

  robot.shooter->setDistanceToFlag(9_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

}
