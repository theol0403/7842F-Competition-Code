#include "7842FMain/Auto/AutoIncludes.hpp"

void AutoRedClose()
{

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
}


void AutoBlueClose()
{

}
