#include "OdomTask.hpp"

void driverOdomTask(void*)
{
  OdomState odomState = robotChassis->getState();

  ControllerButton b_odomToggle(ControllerId::master, ControllerDigital::B);
  bool odomToggled = false;

  ControllerButton b_odomMotionToggle(ControllerId::master, ControllerDigital::A);

  auto profileController = AsyncMotionProfileControllerBuilder()
  .withOutput(robotChassis)
  .withLimits(PathfinderLimits{1.0, 2.0, 10.0})
  .buildMotionProfileController();


  while (true)
  {

    if(b_odomToggle.changedToPressed())
    {
      odomToggled = !odomToggled;
    }

    if(b_odomMotionToggle.changedToPressed())
    {
      profileController->moveTo({
        Point{odomState.x, odomState.y, odomState.theta},
        Point{4_ft, 4_ft, 0_deg}
      });
    }

    if(odomToggled)
    {
      robotChassis->turnAngle(0 - odomState.theta.convert(degree));
    }
    else
    {
      setBaseArcade(j_Main.getAnalog(ControllerAnalog::rightX), j_Main.getAnalog(ControllerAnalog::leftX));
    }


    printf("x: %1.23f, y: %1.23f, theta: %1.23f\n",
    odomState.x.convert(inch),
    odomState.y.convert(inch),
    odomState.theta.convert(degree));


    pros::delay(20);
  }
}
