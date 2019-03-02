#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonPlatformFar(void* input)
{
  SideController* chassis = static_cast<SideController*>(input);

  chassis->setState({1_ft, 3_ft, 90_deg});

  AsyncAction intake = AsyncAction()
  .withTrigger(makeTrigger(return computeDistanceToPoint({4_ft, 3_ft}) < 2_ft;))
  .withMakeAction(robot.intake->setState(IntakeController::intakeBall););

  chassis->driveToPoint({4_ft, 3_ft}, 1, makeSettle(3_in), {intake}); // Move to ball under cap

  chassis->driveToPoint({3.5_ft, 2_ft});

  chassis->turnToAngle(180_deg);

  chassis->driveForTime(3000, -1);

}
