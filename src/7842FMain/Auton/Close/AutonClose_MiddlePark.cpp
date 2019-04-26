#include "7842FMain/AutonIncludes.hpp"

void AutonClose_MiddlePark(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 7_ft);

  chassis.driveToPoint({3_ft, 8_ft}, 1.5, makeSettle(2_in)); // Move to shooting position

  chassis.turnToPoint(middleFlagShoot); // turn to flag
  robot.shooter->setDistanceToFlag(distanceToPoint(middleFlagShoot));
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

  //flip cap
  chassis.driveDistance(1_ft, makeSettle(5_in));
  robot.intake->setState(IntakeController::outIntake);
  chassis.turnToPoint(closeFlatCap);
  chassis.driveDistance(1_ft, makeSettle(4_in));
  robot.intake->setState(IntakeController::intakeBall);
  chassis.driveToPoint({3_ft, 7_ft}, 2, makeSettle(6_in)); // Move to shooting position

  //drive to platform
  chassis.driveToPoint({0.8_ft, 7_ft}, 1, makeSettle(4_in));
  chassis.driveToPoint({0.8_ft, 5_ft}, 2, makeSettle(2_in));
  chassis.turnToAngle(-90_deg);

  //drive up
  robot.arm->setState(ArmController::balance);
  chassis.driveForTime(2000, -1);
  chassis.driveDistance(0_in);

  //shoot middle flags from platform
  // chassis.turnAngle(90_deg);
  // waitForLastMinute(timer);
  // robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBothPlatform);

}
