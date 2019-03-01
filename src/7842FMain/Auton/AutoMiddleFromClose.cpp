#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonMiddleFromClose(void* input)
{
  SideController* chassis = static_cast<SideController*>(input);

  chassis->setState({1_ft, 7_ft, 90_deg}); // Robot is facing cap

  AsyncAction intake = AsyncAction()
  .withTrigger(makeTrigger(return computeDistanceToPoint({4_ft, 7_ft}) < 2_ft;))
  .withMakeAction(robot.intake->setState(IntakeController::intakeBall););

  chassis->driveToPoint({4_ft, 7_ft}, 1, makeSettle(3_in), {intake}); // Move to ball under cap
  chassis->driveToPoint({3_ft, 8_ft}); // Move to behind shooting position

  chassis->turnToPoint(middleFlagShoot); // turn to flag

  robot.shooter->setTarget(0);
  robot.shooter->doMacro(ShootController::shootMacros::shootTarget);
  pros::delay(500);
  robot.shooter->setTarget(5);
  robot.shooter->doMacro(ShootController::shootMacros::shootTarget);
  pros::delay(700);

}
