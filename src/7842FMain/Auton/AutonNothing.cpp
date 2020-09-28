#include "7842FMain/AutonIncludes.hpp"

void AutonNothing(void* input) {}

void AutonTest(void* input) {
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto& [chassis, timer] = *passer;

  // robot.arm->setState(ArmController::carry);

  firstCapMovement(chassis, 7_ft);
  chassis.driveToPoint({startX, 7_ft});

  // robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBothFlags);

  // chassis.turnToAngle(90_deg);
  // chassis.setState({0_ft, 0_ft, 0_deg});
  // chassis.driveDistance(1_ft);
  // chassis.driveDistance(1_ft);
}
