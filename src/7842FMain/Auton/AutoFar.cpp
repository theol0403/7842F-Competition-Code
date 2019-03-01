#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonFar(void* input)
{
  SideController* chassis = static_cast<SideController*>(input);

  chassis->setState({1_ft, 3_ft, 90_deg}); // Robot is facing cap

  AsyncAction intake = AsyncAction()
  .withTrigger(makeTrigger(return computeDistanceToPoint({4_ft, 3_ft}) < 2_ft;))
  .withMakeAction(robot.intake->setState(IntakeController::intakeBall););

  chassis->driveToPoint({4_ft, 3_ft}, 1, makeSettle(3_in), {intake}); // Move to ball under cap
  chassis->driveToPoint({1_ft, 3_ft}); // Move to behind shooting position

  chassis->turnToPoint(sideFlagPost); // turn to flag

  robot.shooter->setDistanceToFlag(computeDistanceToPoint(sideFlagShoot));
  robot.shooter->doMacro(ShootController::shootMacros::shootTopFlag);
  pros::delay(500);
  robot.shooter->doMacro(ShootController::shootMacros::shootMiddleFlag);
  pros::delay(700);

}
