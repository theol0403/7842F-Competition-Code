#include "7842FMain/AutonIncludes.hpp"

void AutonFar_CapNear(void* input) {
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto& [chassis, timer] = *passer;

  firstCapMovement(chassis, 2.9_ft);

  // back up and unlock arm
  robot.arm->setState(ArmController::balance);
  chassis.driveToPoint({3_ft, 2_ft}, 3, makeSettle(2_in));
  robot.arm->setState(ArmController::down);

  // get cap
  chassis.turnAngle(mirrorSide(angleToPoint(farFlatCap), chassis.side) + 180_deg);
  chassis.driveDistance(-2_ft, makeSettle(2_in));
  robot.arm->setState(ArmController::carry);
  pros::delay(300);

  // drive to post
  chassis.driveToPoint({2.5_ft, 1.45_ft}, 2, makeSettle(3_in));
  chassis.turnToPoint(farPost, capTurn);

  robot.arm->setState(ArmController::up);
  chassis.driveForTime(500, 0.6);
  pros::delay(200);
  robot.arm->setState(ArmController::off);

  chassis.driveToPoint({0.5_ft, 3_ft}); // Move to shooting position
  robot.arm->setState(ArmController::down);

  chassis.turnToPoint({2_ft, 11_ft}); // turn to flag
  robot.intake->setState(IntakeController::intakeBall);

  robot.shooter->setDistanceToFlag(9_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);
}
