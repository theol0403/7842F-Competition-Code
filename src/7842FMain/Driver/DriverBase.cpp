#include "DriverBase.hpp"

static ControllerButton b_odomMotionToggle(ControllerId::master, ControllerDigital::A);

void driverBaseControl()
{

  OdomState odomState = robotChassis->getState();

  if(j_Main.getDigital(ControllerDigital::B))
  {
    robotChassis->turnToAngle(0_deg);
  }
  else
  {
    setBaseArcade(j_Main.getAnalog(ControllerAnalog::rightY), j_Main.getAnalog(ControllerAnalog::leftX));
  }

  if(j_Main.getDigital(ControllerDigital::X))
  {
    robotChassis->setState(OdomState{0_ft, 0_ft, 0_deg});
  }

  if(b_odomMotionToggle.changedToPressed())
  {
    robotChassis->driveToPoint(0_ft, 0_ft);
  }


  printf("x: %1.2f, y: %1.2f, theta: %1.2f\n",
  odomState.x.convert(inch),
  odomState.y.convert(inch),
  odomState.theta.convert(degree));


  printf("left: %d, right: %d, middle: %d\n",
  robotChassis->getSensorVals()[0],
  robotChassis->getSensorVals()[1],
  robotChassis->getSensorVals()[2]);

}
