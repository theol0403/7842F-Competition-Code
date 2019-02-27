#include "7842FMain/Auton/AutonIncludes.hpp"


void AutonClose(void* input)
{
  SideController* chassis = static_cast<SideController*>(input);

  chassis->setState({1_ft, 7_ft, 90_deg}); //Robot is facing cap
  robot.intake->setState(IntakeController::intakeBall);

  QLength topShootDistance = 7_ft;

  chassis->driveToPoint({4.5_ft, 7_ft});                  //Move to ball under cap
  chassis->driveToPoint({1_ft, topShootDistance}); //Move to behind shooting position
  chassis->turnToPoint(sideFlagShoot);                         //turn to flag

  robot.shooter->setDistanceToFlag(topShootDistance);
  robot.shooter->doMacro(ShootController::shootMacros::shootBothFlags);
  while(robot.shooter->getCurrentJob() != ShootController::standby) pros::delay(5);

  chassis->driveToPoint({1_ft, 12_ft}); // Move forward towards flags and push bottom flag

  chassis->driveToPoint({0.5_ft, 10.5_ft}); //Push Bottom Flag
  chassis->driveToPoint({0.4_ft, 8.6_ft});  //Move back


  //   setIntakeMode(intakeModes::loading);
  //
  //   if(side == autonSides::red)
  //   {
  //     setFlywheelRPM(2800);
  //     tracker->setState(1_ft, 7_ft, 90_deg);
  //
  //
  //     chassis->driveToPoint({4.5_ft, 7_ft}); //Move to ball under cap
  //     chassis->driveToPoint({1_ft, 7_ft}); //Move to shooting position
  //
  //     chassis->turnToPoint(leftFlag); //Shoot
  //     setIntakeMode(intakeModes::shootIndexer);
  //     pros::delay(200);
  //
  //     chassis->driveToPoint({0.5_ft, 8.8_ft}); // Move forward to second shooting position
  //
  //     chassis->turnToPoint(leftFlag); //Shoot
  //     setIntakeMode(intakeModes::shootIndexer);
  //     pros::delay(200);
  //
  //     chassis->driveToPoint({0.5_ft, 10.5_ft}); //Push Bottom Flag
  //     chassis->driveToPoint({0.4_ft, 8.6_ft}); //Move back
  //
  //     chassis->turnToAngle(-90_deg); //Turn to cap
  //     tracker->setState(1_ft, 9_ft, -90_deg);
  //
  //     //setFlywheelRPM(3000);
  //     chassis->driveToPoint({3.9_ft, 9_ft}); //Flip Cap
  //
  //     // chassis->turnToAngle(45_deg); //Turn to flag
  //     // setIntakeMode(intakeModes::shootIndexer);
  //     // pros::delay(200);
  //   }
  //
  //   else if(side == autonSides::blue)
  //   {
  //     setFlywheelRPM(2800);
  //     tracker->setState(FX 1_ft, 7_ft, FA 90_deg);
  //
  //
  //     chassis->driveToPoint({FX 4.5_ft, 7_ft}); //Move to ball under cap
  //     chassis->driveToPoint({FX 1.1_ft, 7_ft}); //Move to shooting position
  //
  //     chassis->turnToPoint(rightFlag); //Shoot first ball
  //     setIntakeMode(intakeModes::shootIndexer);
  //     pros::delay(200);
  //
  //     tracker->setX(FX 1_ft);
  //     chassis->driveToPoint({FX 0.7_ft, 10_ft}); // Move forward to second shooting position
  //
  //     chassis->turnToPoint(rightFlag); //Shoot second ball
  //     setIntakeMode(intakeModes::shootIndexer);
  //     pros::delay(200);
  //
  //     chassis->driveToPoint({FX 0.6_ft, 11.5_ft}); //Push Bottom Flag
  //     chassis->driveToPoint({FX 1_ft, 9.5_ft}); //Move back
  //
  //     chassis->turnToAngle(FA -90_deg); //Turn to cap
  //     tracker->setState(FX 1_ft, 9_ft, FA -90_deg); //Set state before cap
  //
  //     setFlywheelRPM(3000);
  //     chassis->driveToPoint({FX 3.9_ft, 9_ft}); //Flip Cap
  //
  //     chassis->turnToAngle(FA 45_deg); //Turn to flag
  //
  //     setIntakeMode(intakeModes::shootIndexer);
  //     pros::delay(200);
  //   }

}
