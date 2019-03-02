#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonCloseExperimental(void* input)
{
  SideController* chassis = static_cast<SideController*>(input);

  chassis->setState({1_ft, 7_ft, 90_deg}); // Robot is facing cap

  AsyncAction intake = AsyncAction()
  .withTrigger(makeTrigger(return computeDistanceToPoint({4_ft, 7_ft}) < 2_ft;))
  .withMakeAction(robot.intake->setState(IntakeController::intakeBall););

  chassis->driveToPoint({4_ft, 7_ft}, 1, makeSettle(3_in), {intake}); // Move to ball under cap
  chassis->driveToPoint({3_ft, 7.5_ft}, 1, makeSettle(2_in)); // Move to shooting position

  chassis->turnToPoint(middleFlagShoot); // turn to flag

  robot.shooter->setTarget(0);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTarget);
  robot.shooter->setTarget(20);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTarget);

  chassis->turnToPoint(closeFlatCap);
  robot.arm->setState(ArmController::down);
  chassis->driveDistance(-3_in);

  chassis->driveToPoint({1_ft, 7_ft}, 1, makeSettle(3_in)); // Move to behind shooting position
  chassis->turnToPoint(sideFlagShoot); // turn to flag

  robot.shooter->setTarget(0);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTarget);
  robot.shooter->setTarget(20);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTarget);

}
