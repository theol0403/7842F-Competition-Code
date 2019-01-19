#include "7842FMain/Auto/AutoIncludes.hpp"


void AutoClose(lib7842::autonSides side)
{
  setIntakeMode(intakeModes::loading);

  if(side == lib7842::autonSides::red)
  {
    setFlywheelRPM(2700);
    chassisOdomTracker->setState(1_ft, 7_ft, 90_deg);


    chassisOdomController->driveToPoint(lib7842::Point{4.5_ft, 7_ft}); //Move to ball under cap
    chassisOdomController->driveToPoint(lib7842::Point{1_ft, 7.5_ft}); //Move to shooting position

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

  else if(side == lib7842::autonSides::blue)
  {
    setFlywheelRPM(2700);
    chassisOdomTracker->setState(FX 1_ft, 7_ft, FA 90_deg);


    chassisOdomController->driveToPoint(lib7842::Point{FX 4.5_ft, 7_ft}); //Move to ball under cap
    chassisOdomController->driveToPoint(lib7842::Point{FX 1.1_ft, 7.5_ft}); //Move to shooting position

    chassisOdomController->m_odomTracker->state.theta += 5_deg;
    chassisOdomController->turnToPoint(rightFlag); //Shoot first ball
    setIntakeMode(intakeModes::shootIndexer);
    pros::delay(200);

    chassisOdomController->m_odomTracker->setX(FX 1_ft);
    chassisOdomController->driveToPoint(lib7842::Point{FX 0.7_ft, 10_ft}); // Move forward to second shooting position

    chassisOdomController->m_odomTracker->state.theta += 5_deg;
    chassisOdomController->turnToPoint(rightFlag); //Shoot second ball
    setIntakeMode(intakeModes::shootIndexer);
    pros::delay(200);
    chassisOdomController->m_odomTracker->state.theta -= 3_deg;

    chassisOdomController->driveToPoint(lib7842::Point{FX 0.6_ft, 11.2_ft}); //Push Bottom Flag
    chassisOdomController->driveToPoint(lib7842::Point{FX 1_ft, 9.5_ft}); //Move back

    chassisOdomController->m_odomTracker->state.theta -= 5_deg;
    chassisOdomController->turnToAngle(FA -90_deg); //Turn to cap
    chassisOdomTracker->setState(FX 1_ft, 9_ft, FA -90_deg); //Set state before cap

    setFlywheelRPM(3000);
    chassisOdomController->driveToPoint(lib7842::Point{FX 3_ft, 9_ft}); //Flip Cap

    chassisOdomController->turnToAngle(FA 45_deg); //Turn to flag

    setIntakeMode(intakeModes::shootIndexer);
    pros::delay(200);
  }

}
