#include "7842FMain/AutonIncludes.hpp"

void AutonClose_NearCross(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 7_ft);

  closeShootFlagsPush(chassis);

  chassis.driveDistance(-3_ft, makeSettle(4_in));

  robot.intake->setState(IntakeController::outIntake);
  chassis.driveToPoint({3_ft, 8.5_ft}, 2, makeSettle(2_in)); // Move to shooting position
  robot.intake->setState(IntakeController::intakeBall);

  chassis.driveToPoint({4_ft, 7_ft}, 2, makeSettle(2_in)); // Move to shooting position

  chassis.turnToPoint(farFlagShoot); // turn to flag

  robot.shooter->setDistanceToFlag(distanceToPoint(farFlagShoot));
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);


}
