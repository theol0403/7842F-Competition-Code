#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonMiddleFromFar(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  firstCapMovement(chassis, 3_ft);

  chassis.driveToPoint({0.9_ft, 3_ft}, 1, makeSettle(3_in)); // Move to shooting position

  if(chassis.side == autonSides::blue) {
    chassis.turnToPoint({4.8_ft, 11.2_ft}); // turn to flag
  } else {
    chassis.turnToPoint(middleFlagShoot); // turn to flag
  }


  waitForLastMinute(timer);

  robot.shooter->setDistanceToFlag(computeDistanceToPoint(middleFlagShoot));
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

}
