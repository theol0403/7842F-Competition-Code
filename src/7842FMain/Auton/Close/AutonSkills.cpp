#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonSkills(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  chassis.setState({startX, 7_ft, 90_deg}); // Robot is facing cap
  robot.intake->setState(IntakeController::intakeBall);

  //this will make the robot intake when it is a distance from the cap
  AsyncAction intake = AsyncAction()
  .withTrigger(makeTrigger(return computeDistanceToPoint({4_ft, 7_ft}) < 2_ft;))
  .withMakeAction(robot.intake->setState(IntakeController::outIntake););

  // Move to ball under cap while intaking
  chassis.driveToPoint({4_ft, 7_ft}, 1, driveSettle, {intake});
  robot.intake->setState(IntakeController::intakeBall);
  chassis.driveDistance(0.5_ft);
  pros::delay(2000);

  chassis.driveToPoint({0.9_ft, 7_ft}, 1.5, makeSettle(2_in)); // Move to shooting position

  chassis.turnToPoint(sideFlagShoot); // turn to flag
  robot.shooter->setDistanceToFlag(7_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

  // Move forward towards flags and push bottom flag
  chassis.turnToAngle(1.5_deg);
  chassis.driveDistance(3.7_ft);
  chassis.driveDistance(-1.5_ft, makeSettle(0.5_ft));
  chassis.turnToAngle(90_deg);

  robot.intake->setState(IntakeController::outIntake);
  chassis.driveToPoint({3_ft, 8_ft}, 1, makeSettle(2_in));

  robot.intake->setState(IntakeController::intakeBall);
  pros::delay(1000);

  chassis.turnToPoint(middleFlagShoot);

  robot.shooter->setDistanceToFlag(5.5_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);
  pros::delay(4000);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

}
