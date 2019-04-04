#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonPlatformFar(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
auto &[chassis, timer] = *passer;

  chassis.setState({1_ft, 3_ft, 90_deg});

  AsyncAction intake = AsyncAction()
  .withTrigger(makeTrigger(return computeDistanceToPoint(farCapDrive) < 2_ft;))
  .withMakeAction(robot.intake->setState(IntakeController::intakeBall););

  chassis.driveToPoint(farCapDrive, 1, makeSettle(3_in), {intake}); // Move to ball under cap

  chassis.driveToPoint({3.93_ft, 2.5_ft});

  chassis.turnToAngle(180_deg);

  chassis.driveForTime(1600, -1);

}
