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
  chassis.driveToPoint({3.9_ft, 7_ft}, 1, driveSettle, {intake});
  robot.intake->setState(IntakeController::intakeBall);
  chassis.driveDistance(0.5_ft);
  pros::delay(1000);

  chassis.driveToPoint({0.9_ft, 7_ft}, 1.5, makeSettle(2_in)); // Move to shooting position
  robot.intake->setState(IntakeController::intakeBall);

  chassis.turnToPoint(sideFlagShoot); // turn to flag
  robot.shooter->setDistanceToFlag(7_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

  // Move forward towards flags and push bottom flag
  chassis.turnToAngle(1.5_deg);
  chassis.driveDistance(3.8_ft);
  chassis.driveDistance(-1.5_ft, makeSettle(0.5_ft));
  chassis.turnToAngle(90_deg);

  robot.intake->setState(IntakeController::outIntake);
  chassis.driveToPoint({3_ft, 8_ft}, 1, makeSettle(2_in));

  robot.intake->setState(IntakeController::intakeBall);
  pros::delay(1000);

  chassis.turnToPoint(middleFlagShoot);

  robot.shooter->setDistanceToFlag(5.5_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);
  pros::delay(3000);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

  chassis.driveDistance(2.4_ft);

  chassis.driveToPoint({0.8_ft, 5_ft}, 1, makeSettle(2_in));
  chassis.turnToAngle(-95_deg);

  chassis.driveForTime(2000, -1);
  chassis.driveDistance(-0.1_ft);

}
