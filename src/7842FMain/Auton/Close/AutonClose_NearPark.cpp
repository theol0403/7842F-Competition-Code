#include "7842FMain/AutonIncludes.hpp"

void AutonClose_NearPark(void* input) {
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto& [chassis, timer] = *passer;

  firstCapMovement(chassis, 7_ft);

  closeShootFlagsPush(chassis);

  // drive to platform
  chassis.driveToPoint({0.8_ft, 5_ft}, 1, makeSettle(2_in));
  chassis.turnToAngle(-90_deg);

  // drive up
  robot.arm->setState(ArmController::balance);
  chassis.driveForTime(2000, -1);
  chassis.driveDistance(0_in);

  // shoot middle flags from platform
  // chassis.turnAngle(90_deg);
  // waitForLastMinute(timer);
  // robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBothPlatform);
}
