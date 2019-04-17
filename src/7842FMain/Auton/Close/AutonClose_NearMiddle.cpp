#include "7842FMain/AutonIncludes.hpp"

void AutonClose_NearMiddle(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 7_ft);

  chassis.driveToPoint({0.9_ft, 7_ft}, 2); // Move to shooting position

  chassis.turnToPoint(sideFlagShoot); // turn to flag
  robot.shooter->setDistanceToFlag(7_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

  // Move forward towards flags and push bottom flag
  chassis.driveToPoint({1_ft, 11_ft}, 3, makeSettle(4_in));

  chassis.driveDistance(-3_ft, makeSettle(4_in));

  robot.intake->setState(IntakeController::outIntake);
  chassis.driveToPoint({3_ft, 8.5_ft}, 2, makeSettle(2_in)); // Move to shooting position
  robot.intake->setState(IntakeController::intakeBall);

  chassis.turnToPoint(middleFlagShoot); // turn to flag

  robot.shooter->setDistanceToFlag(distanceToPoint(middleFlagShoot));
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

  chassis.turnAngle(-5_deg);
  chassis.driveDistance(3_ft);
  chassis.driveDistance(-2_ft);

}
