#include "7842FMain/AutonIncludes.hpp"

void AutonClose_NearMiddle(void* input) {
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto& [chassis, timer] = *passer;

  firstCapMovement(chassis, 7_ft);

  closeShootFlagsPush(chassis);

  chassis.driveDistance(-3_ft, makeSettle(4_in));

  AsyncAction outtake = chassis.makeAsyncAction()
                          .withTrigger(closeFlatCap, 3_ft, 45_deg)
                          .withMakeAction(robot.intake->setState(IntakeController::outIntake););
  chassis.driveToPoint({3.3_ft, 8.5_ft}, 2, makeSettle(2_in),
                       {outtake}); // Move to shooting position
  robot.intake->setState(IntakeController::intakeBall);

  chassis.turnToPoint(middleFlagShoot); // turn to flag

  robot.shooter->setDistanceToFlag(distanceToPoint(middleFlagShoot));
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

  // chassis.driveDistance(2_ft, makeSettle(2_in));
  // chassis.turnToPoint(middleFlagShoot); // turn to flag

  // robot.shooter->setDistanceToFlag(0_ft);
  pros::delay(500);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootMiddle);
}
