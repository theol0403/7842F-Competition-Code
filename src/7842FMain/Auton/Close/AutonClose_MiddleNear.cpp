#include "7842FMain/AutonIncludes.hpp"

void AutonClose_MiddleNear(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 7_ft);

  chassis.driveToPoint({3_ft, 8_ft}, 1.5, makeSettle(2_in)); // Move to shooting position

  chassis.turnToPoint(middleFlagShoot); // turn to flag

  robot.shooter->setDistanceToFlag(distanceToPoint(middleFlagShoot));
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

  chassis.driveDistance(2_ft, makeSettle(5_in));

  chassis.turnToPoint(closeFlatCap);
  robot.intake->setState(IntakeController::outIntake);
  chassis.driveDistance(2_ft, makeSettle(5_in));
  robot.intake->setState(IntakeController::intakeBall);

  chassis.driveToPoint({0.9_ft, 7_ft}, 0.5); // Move to shooting position
  chassis.turnToPoint(sideFlagShoot); // turn to flag

  // Move forward towards flags and push bottom flag
  chassis.driveToPoint({1_ft, 11_ft}, 2, makeSettle(4_in));

  chassis.driveToPoint({0.9_ft, 7_ft}, 2); // Move to shooting position

  chassis.turnToPoint(sideFlagShoot); // turn to flag
  robot.shooter->setDistanceToFlag(7_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);


}
