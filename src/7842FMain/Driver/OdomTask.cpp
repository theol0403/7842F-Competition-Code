#include "OdomTask.hpp"

void driverOdomTask(void*)
{
  std::shared_ptr<okapi::AsyncMotionProfileController> robotProfile = AsyncMotionProfileControllerBuilder()
  .withOutput(robotChassis)
  .withLimits(PathfinderLimits{1.0, 2.0, 10.0})
  .buildMotionProfileController();


  ControllerButton b_odomToggle(ControllerId::master, ControllerDigital::B);
  bool odomToggled = false;

  ControllerButton b_odomMotionToggle(ControllerId::master, ControllerDigital::A);




  while (true)
  {
    OdomState odomState = robotChassis->getState();

    if(b_odomToggle.changedToPressed())
    {
      odomToggled = !odomToggled;
    }

    if(b_odomMotionToggle.changedToPressed())
    {
      robotProfile->moveTo({
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
      setBaseArcade(j_Main.getAnalog(ControllerAnalog::rightY), j_Main.getAnalog(ControllerAnalog::leftX));
    }

    if(j_Main.getDigital(ControllerDigital::X))
    {
      robotChassis->setState(OdomState{0_ft, 0_ft, 0_deg});
    }


    printf("x: %1.2f, y: %1.2f, theta: %1.2f\n",
    odomState.x.convert(inch),
    odomState.y.convert(inch),
    odomState.theta.convert(degree));

    ADIEncoder leftEncoder(3, 4);
    ADIEncoder rightEncoder(5, 6);
    ADIEncoder middleEncoder(7, 8);

      printf("left: %1.2f, right: %1.2f, middle: %1.2f\n",
      leftEncoder.get(),
      rightEncoder.get(),
      middleEncoder.get());


    pros::delay(100);
  }
}
