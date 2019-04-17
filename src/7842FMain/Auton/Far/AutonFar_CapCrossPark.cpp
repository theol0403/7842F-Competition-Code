#include "7842FMain/AutonIncludes.hpp"

void AutonFar_CapCrossPark(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 3_ft);

  robot.arm->setState(ArmController::balance);

  chassis.driveToPoint({3_ft, 3_ft}, 1, makeSettle(2_in));

  robot.intake->setState(IntakeController::off);
  robot.arm->setState(ArmController::down);

  chassis.turnAngle(mirrorSide(angleToPoint(farFlatCap), chassis.side) + 180_deg);
  chassis.driveDistance(-2_ft, makeSettle(2_in));
  robot.arm->setState(ArmController::carry);
  pros::delay(300);

  chassis.driveToPoint({2.5_ft, 1.3_ft}, 2, makeSettle(3_in));
  chassis.turnToPoint(farPost, capTurn);
  chassis.driveForTime(500, 0.5);

  robot.arm->setState(ArmController::up);
  pros::delay(800);
  //chassis.setState({3_ft, 16_in, 180_deg});
  robot.arm->setState(ArmController::off);
  chassis.driveDistance(-1_ft, makeSettle(2_in));
  robot.arm->setState(ArmController::down);

  if(chassis.side == autonSides::red) {
    chassis.turnToPoint({12.3_ft, 11_ft}); // turn to flag
  } else {
    chassis.turnToPoint({10.9_ft, 11_ft}); // turn to flag
  }

  robot.shooter->setDistanceToFlag(9.5_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTop);
  pros::delay(1000);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootMiddle);



}
