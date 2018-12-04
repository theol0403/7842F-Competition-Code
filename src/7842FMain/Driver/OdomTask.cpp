#include "OdomTask.hpp"

void driverOdomTask(void*)
{
  ControllerButton odomToggle(ControllerId::master, ControllerDigital::B);
  bool odomToggled = false;

  OdomState odomState = robotChassis->getState();

  while (true)
  {

    if(odomToggle.changedToPressed())
    {
      odomToggled = !odomToggled;
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
