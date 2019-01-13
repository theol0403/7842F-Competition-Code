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

    lib7842::Point leftFlag{0.8_ft, 11.5_ft};

    setFlywheelRPM(2800);
    setIntakeMode(intakeModes::loading);

    chassisOdomTracker->setState(1_ft, 7_ft, 90_deg);
    chassisOdomController->driveToPoint(lib7842::Point{4.5_ft, 7_ft});
    chassisOdomController->driveToPoint(lib7842::Point{1_ft, 6.5_ft});

    chassisOdomController->turnToPoint(leftFlag);

    setIntakeMode(intakeModes::shootIndexer);
    pros::delay(200);

    chassisOdomController->m_odomTracker->state.theta += 5_deg;
    chassisOdomController->driveToPoint(lib7842::Point{0.5_ft, 8.2_ft});

    chassisOdomController->turnToPoint(leftFlag);

    setIntakeMode(intakeModes::shootIndexer);
    pros::delay(200);

    chassisOdomController->m_odomTracker->state.theta += 3_deg;
    chassisOdomController->driveToPoint(lib7842::Point{0.5_ft, 10.3_ft});
    chassisOdomController->driveToPoint(lib7842::Point{0.4_ft, 8.2_ft});

    chassisOdomController->turnToAngle(-90_deg);
    chassisOdomTracker->setState(1_ft, 9_ft, -90_deg);
    chassisOdomController->driveToPoint(lib7842::Point{3.4_ft, 9_ft});

    chassisOdomController->driveToPoint(lib7842::Point{1.6_ft, 7_ft});

    chassisOdomController->turnToAngle(0_deg);
    chassisOdomController->m_odomTracker->state.x = 1_in;
    chassisOdomController->m_odomTracker->state.y = 6.5_in;

    chassisOdomController->driveToPointAndAngle(lib7842::Point{1_ft, 5_ft, -90_deg});

    chassisOdomController->m_chassisController->stop();
    chassisOdomController->m_chassisController->arcade(200, 0);
    pros::delay(2000);
    chassisOdomController->m_chassisController->stop();











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
