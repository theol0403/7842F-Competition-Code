#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonMiddleFromFar(void* input)
{
  SideController* chassis = static_cast<SideController*>(input);

  chassis->setState({1_ft, 3_ft, 90_deg}); // Robot is facing cap

  AsyncAction intake = AsyncAction()
  .withTrigger(makeTrigger(return computeDistanceToPoint({4_ft, 3_ft}) < 2_ft;))
  .withMakeAction(robot.intake->setState(IntakeController::intakeBall););

  chassis->driveToPoint(farCapDrive 1, makeSettle(3_in), {intake}); // Move to ball under cap
  chassis->driveToPoint({3_ft, 2_ft}, 1.5, makeSettle(2_in)); // Move to shooting position

  chassis->turnToPoint(middleFlagShoot); // turn to flag

  robot.shooter->setDistanceToFlag(computeDistanceToPoint(middleFlagShoot));
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTopFlag);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootMiddleFlag);

}
