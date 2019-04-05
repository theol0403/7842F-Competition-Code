#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonPlatformFar(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  robot.flywheel->setRpm(0);

  firstCapMovement(chassis, 3_ft);

  chassis.driveToPoint({3.93_ft, 2.5_ft});

  chassis.turnToAngle(180_deg);

  chassis.driveForTime(1600, -1);

}
