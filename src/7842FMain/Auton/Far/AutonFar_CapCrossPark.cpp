#include "7842FMain/AutonIncludes.hpp"

void AutonFar_CapCrossPark(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 3_ft);

  chassis.driveToPoint({2.8_ft, 3_ft}, 1, makeSettle(2_in)); // Move to shooting position
  chassis.turnAngle(mirrorSide(angleToPoint(farFlatCap), chassis.side) + 180_deg, pointTurn, makeSettle(2_deg));
  chassis.driveDistance(-2_ft);
  robot.arm->setState(ArmController::carry);
  pros::delay(500);

  chassis.driveToPoint({2_ft, 2_ft});
  chassis.turnToAngle(180_deg);



}
