#include "7842FMain/Auto/AutoIncludes.hpp"


void AutoClose(lib7842::autonSides side)
{
  setIntakeMode(intakeModes::loading);

  // if(side == lib7842::autonSides::red)
  // {
  //   setFlywheelRPM(2800);
  //   tracker->setState(1_ft, 7_ft, 90_deg);
  //
  //
  //   chassis->driveToPoint(lib7842::Point{4.5_ft, 7_ft}); //Move to ball under cap
  //   chassis->driveToPoint(lib7842::Point{1_ft, 7_ft}); //Move to shooting position
  //
  //   chassis->turnToPoint(leftFlag); //Shoot
  //   setIntakeMode(intakeModes::shootIndexer);
  //   pros::delay(200);
  //
  //   chassis->m_odomTracker->state.theta += 5_deg;
  //   chassis->driveToPoint(lib7842::Point{0.5_ft, 8.8_ft}); // Move forward to second shooting position
  //
  //   chassis->turnToPoint(leftFlag); //Shoot
  //   setIntakeMode(intakeModes::shootIndexer);
  //   pros::delay(200);
  //
  //   chassis->m_odomTracker->state.theta += 3_deg;
  //   chassis->driveToPoint(lib7842::Point{0.5_ft, 10.5_ft}); //Push Bottom Flag
  //   chassis->driveToPoint(lib7842::Point{0.4_ft, 8.6_ft}); //Move back
  //
  //   chassis->turnToAngle(-90_deg); //Turn to cap
  //   tracker->setState(1_ft, 9_ft, -90_deg);
  //
  //   //setFlywheelRPM(3000);
  //   chassis->driveToPoint(lib7842::Point{3.9_ft, 9_ft}); //Flip Cap
  //
  //   // chassis->turnToAngle(45_deg); //Turn to flag
  //   // setIntakeMode(intakeModes::shootIndexer);
  //   // pros::delay(200);
  // }
  //
  // else if(side == lib7842::autonSides::blue)
  // {
  //   setFlywheelRPM(2800);
  //   tracker->setState(FX 1_ft, 7_ft, FA 90_deg);
  //
  //
  //   chassis->driveToPoint(lib7842::Point{FX 4.5_ft, 7_ft}); //Move to ball under cap
  //   chassis->driveToPoint(lib7842::Point{FX 1.1_ft, 7_ft}); //Move to shooting position
  //
  //   chassis->m_odomTracker->state.theta += 5_deg;
  //   chassis->turnToPoint(rightFlag); //Shoot first ball
  //   setIntakeMode(intakeModes::shootIndexer);
  //   pros::delay(200);
  //
  //   chassis->m_odomTracker->setX(FX 1_ft);
  //   chassis->driveToPoint(lib7842::Point{FX 0.7_ft, 10_ft}); // Move forward to second shooting position
  //
  //   chassis->m_odomTracker->state.theta += 5_deg;
  //   chassis->turnToPoint(rightFlag); //Shoot second ball
  //   setIntakeMode(intakeModes::shootIndexer);
  //   pros::delay(200);
  //   chassis->m_odomTracker->state.theta -= 3_deg;
  //
  //   chassis->driveToPoint(lib7842::Point{FX 0.6_ft, 11.5_ft}); //Push Bottom Flag
  //   chassis->driveToPoint(lib7842::Point{FX 1_ft, 9.5_ft}); //Move back
  //
  //   chassis->m_odomTracker->state.theta -= 5_deg;
  //   chassis->turnToAngle(FA -90_deg); //Turn to cap
  //   tracker->setState(FX 1_ft, 9_ft, FA -90_deg); //Set state before cap
  //
  //   setFlywheelRPM(3000);
  //   chassis->driveToPoint(lib7842::Point{FX 3.9_ft, 9_ft}); //Flip Cap
  //
  //   chassis->turnToAngle(FA 45_deg); //Turn to flag
  //
  //   setIntakeMode(intakeModes::shootIndexer);
  //   pros::delay(200);
  // }

}
