#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonClose(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 7_ft);

  chassis.driveToPoint({0.9_ft, 7_ft}, 2); // Move to shooting position

  chassis.turnToPoint(sideFlagShoot); // turn to flag
  robot.shooter->setDistanceToFlag(7_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

  // Move forward towards flags and push bottom flag
  chassis.driveToPoint({1_ft, 10.5_ft}, 3, makeSettle(4_in));

  chassis.driveToPoint({1_ft, 9_ft}, 1, makeSettle(4_in));
  chassis.turnToPoint(closeFlatCap);

  chassis.driveDistance(1_ft, makeSettle(2_in)); //forward
  chassis.driveDistance(-1_ft, makeSettle(2_in)); //back
  pros::delay(500);
  robot.intake->setState(IntakeController::outIntake); //flip
  chassis.driveDistance(2_ft, makeSettle(2_in)); //forward

  robot.intake->setState(IntakeController::intakeBall);
  chassis.turnToPoint(middleFlagShoot);

  waitForLastMinute(timer);
  robot.shooter->setDistanceToFlag(5.5_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

}
