#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonClose(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 7_ft);

  chassis.driveToPoint({0.9_ft, 7_ft}, 1.5, makeSettle(2_in)); // Move to shooting position

  chassis.turnToPoint(sideFlagShoot); // turn to flag
  robot.shooter->setDistanceToFlag(7_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

  // Move forward towards flags and push bottom flag
  chassis.turnToAngle(2_deg);
  chassis.driveDistance(3.7_ft);
  chassis.driveDistance(-1.5_ft, makeSettle(0.5_ft));
  chassis.turnToAngle(90_deg);

  robot.intake->setState(IntakeController::outIntake);
  chassis.driveToPoint({3_ft, 9_ft}, 1, makeSettle(2_in));

  robot.intake->setState(IntakeController::intakeBall);
  pros::delay(500);

  chassis.turnToPoint(middleFlagShoot);

  waitForLastMinute(timer);
  robot.shooter->setDistanceToFlag(5.5_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

}
