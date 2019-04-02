#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonMiddleFromClose(void* input)
{
  SideController* chassis = static_cast<SideController*>(input);

  chassis->setState({1_ft, 7_ft, 90_deg}); // Robot is facing cap

  AsyncAction intake = AsyncAction()
  .withTrigger(makeTrigger(return computeDistanceToPoint(closeCapDrive) < 2_ft;))
  .withMakeAction(robot.intake->setState(IntakeController::intakeBall););

  chassis->driveToPoint(closeCapDrive, 1, makeSettle(3_in), {intake}); // Move to ball under cap
  chassis->driveToPoint({3_ft, 8_ft}, 1.5, makeSettle(2_in)); // Move to shooting position

  chassis->turnToPoint(middleFlagShoot); // turn to flag

  robot.shooter->setTarget(0);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTarget);
  robot.shooter->setTarget(20);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTarget);

}
