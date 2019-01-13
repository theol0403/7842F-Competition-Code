#include "DriverBase.hpp"

//static okapi::ControllerButton b_odomMotionToggle(ControllerId::master, ControllerDigital::A);

void driverJoystick()
{
  double rightY = j_Analog(rightY);
  double leftX = j_Analog(leftX);

  //std::cout << "Old: " << leftX;

  // if(true)
  // {
  //   //  rightY = lib7842::sgn(rightY) * pow(abs(rightY), 2);
  //   leftX = lib7842::sgn(leftX) * pow(abs(leftX), 2);
  // }

  //std::cout << " New: " << leftX << std::endl;

  setBaseArcade(rightY, leftX);
}

void driverBaseControl()
{


  if(j_Digital(A))
  {
    chassisOdomController->driveToPoint(lib7842::Point{0_in, 24_in});
    chassisOdomController->driveToPoint(lib7842::Point{0_in, 0_in});
    //#include "7842FMain/Auto/AutoRedClose.hpp"
    // chassisOdomController->driveToPointAndAngle(lib7842::Point{0_in, 10_in, 0_deg});
    // chassisOdomController->turnToAngle(90_deg);
  }

  driverJoystick();

  // if(b_odomMotionToggle.changedToPressed())
  // {
  //   robotChassis->driveToPoint(0_ft, 0_ft);
  // }

  if(j_Digital(B))
  {
    chassisOdomTracker->resetState();
    chassisOdomTracker->resetSensors();
  }

}
