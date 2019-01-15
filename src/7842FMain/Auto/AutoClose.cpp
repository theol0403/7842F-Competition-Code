#include "7842FMain/Auto/AutoIncludes.hpp"


void AutoRedClose()
{
  setFlywheelRPM(2800);
  setIntakeMode(intakeModes::loading);
  chassisOdomTracker->setState(1_ft, 7_ft, 90_deg);

  chassisOdomController->driveToPoint(lib7842::Point{4.5_ft, 7_ft}); //Move to ball under cap
  chassisOdomController->driveToPoint(lib7842::Point{1_ft, 6.5_ft}); //Move to shooting position

  chassisOdomController->turnToPoint(leftFlag); //Shoot
  setIntakeMode(intakeModes::shootIndexer);
  pros::delay(200);

  chassisOdomController->m_odomTracker->state.theta += 5_deg;
  chassisOdomController->driveToPoint(lib7842::Point{0.5_ft, 8.2_ft}); // Move forward to second shooting position

  chassisOdomController->turnToPoint(leftFlag); //Shoot
  setIntakeMode(intakeModes::shootIndexer);
  pros::delay(200);

  chassisOdomController->m_odomTracker->state.theta += 3_deg;
  chassisOdomController->driveToPoint(lib7842::Point{0.5_ft, 10.3_ft}); //Push Bottom Flag
  chassisOdomController->driveToPoint(lib7842::Point{0.4_ft, 8.2_ft}); //Move back

  chassisOdomController->turnToAngle(-90_deg); //Turn to cap
  chassisOdomTracker->setState(1_ft, 9_ft, -90_deg);

  chassisOdomController->driveToPoint(lib7842::Point{3.4_ft, 9_ft}); //Flip Cap
}


void AutoBlueClose()
{

  setFlywheelRPM(2800);
  setIntakeMode(intakeModes::loading);
  chassisOdomTracker->setState(FX 1_ft, 7_ft, FA 90_deg);

  chassisOdomController->driveToPoint(lib7842::Point{FX 4.5_ft, 7_ft}); //Move to ball under cap
  chassisOdomController->driveToPoint(lib7842::Point{FX 1.1_ft, 6.5_ft}); //Move to shooting position

  chassisOdomController->m_odomTracker->state.theta += 5_deg;
  chassisOdomController->turnToPoint(rightFlag); //Shoot first ball
  setIntakeMode(intakeModes::shootIndexer);
  pros::delay(200);

  chassisOdomController->m_odomTracker->setX(FX 1_ft);
  chassisOdomController->driveToPoint(lib7842::Point{FX 0.7_ft, 9_ft}); // Move forward to second shooting position

  chassisOdomController->turnToPoint(rightFlag); //Shoot second ball
  setIntakeMode(intakeModes::shootIndexer);
  pros::delay(200);

  chassisOdomController->driveToPoint(lib7842::Point{FX 0.6_ft, 11.1_ft}); //Push Bottom Flag
  chassisOdomController->driveToPoint(lib7842::Point{FX 0.6_ft, 9.5_ft}); //Move back

  chassisOdomController->turnToAngle(FA -90_deg); //Turn to cap
  chassisOdomTracker->setState(FX 1_ft, 9_ft, FA -90_deg);

  chassisOdomController->driveToPoint(lib7842::Point{FX 3.4_ft, 9_ft}); //Flip Cap

}
