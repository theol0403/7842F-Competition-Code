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
    autonSelector->run();
    // robotChassis->stop();
    // chassis->m_chassisController->stop();
    // chassis->m_chassisController->moveDistance(1_cm);
    // robotChassis->stop();
    // chassis->m_chassisController->stop();

  }

  driverJoystick();

  // if(b_odomMotionToggle.changedToPressed())
  // {
  //   robotChassis->driveToPoint(0_ft, 0_ft);
  // }

  if(j_Digital(B))
  {
    tracker->resetState();
    tracker->resetSensors();
  }

}
