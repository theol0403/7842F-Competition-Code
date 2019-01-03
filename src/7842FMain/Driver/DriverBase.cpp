#include "DriverBase.hpp"

//static okapi::ControllerButton b_odomMotionToggle(ControllerId::master, ControllerDigital::A);

void driverJoystick()
{
  double rightY = j_Analog(rightY);
  double leftX = j_Analog(leftX);

  //std::cout << "Old: " << leftX;

  if(true)
  {
  //  rightY = lib7842::sgn(rightY) * pow(abs(rightY), 2);
    leftX = lib7842::sgn(leftX) * pow(abs(leftX), 2);
  }

  //std::cout << " New: " << leftX << std::endl;

  setBaseArcade(rightY, leftX);
}

void driverBaseControl()
{

  okapi::OdomState odomState = robotChassis->getState();

  // if(j_Main.getDigital(okapi::ControllerDigital::B))
  // {
  //   robotChassis->turnToAngle(0_deg);
  // }
  // else
  // {
  //   driverJoystick();
  // }

  driverJoystick();

  // if(b_odomMotionToggle.changedToPressed())
  // {
  //   robotChassis->driveToPoint(0_ft, 0_ft);
  // }

  if(j_Digital(B))
  {
    robotChassis->setState({0_ft, 0_ft, 0_deg});
    robotChassis->resetSensors();
  }


  printf("x: %1.2f, y: %1.2f, theta: %1.2f\n",
  odomState.x.convert(centimeter),
  odomState.y.convert(centimeter),
  odomState.theta.convert(degree));


  printf("left: %d, right: %d, middle: %d\n",
  robotChassis->getSensorVals()[0],
  robotChassis->getSensorVals()[1],
  robotChassis->getSensorVals()[2]);

}
