#include "7842FMain/Auton/AutonIncludes.hpp"


void AutonClose(void* input)
{
  SideController* chassis = static_cast<SideController*>(input);

  chassis->setState({1_ft, 7_ft, 90_deg}); //Robot is facing cap

  AsyncAction intake = AsyncAction()
  .withTrigger(makeTrigger(return computeDistanceToPoint({4_ft, 7_ft}) < 2_ft;))
  .withMakeAction(robot.intake->setState(IntakeController::intakeBall););

  chassis->driveDistanceAtAngle(3_ft, angleCalc(90_deg), 0.5, makeSettle(2_in), {intake});                  //Move to ball under cap
  chassis->driveDistanceAtAngle(-3_ft, angleCalc(90_deg), 0.5); //Move to behind shooting position
  chassis->turnToPoint(sideFlagShoot);                         //turn to flag

  robot.shooter->setDistanceToFlag(0_ft);
  robot.shooter->doMacro(ShootController::shootMacros::shootBothFlags);
  while(robot.shooter->getCurrentJob() != ShootController::standby) pros::delay(5);

  chassis->driveToPointSimple({1.5_ft, 10.2_ft}, makeSettle(2_in)); // Move forward towards flags and push bottom flag
  chassis->driveDistance(-2_ft, makeSettle(2_in));
  chassis->turnToAngle(90_deg);
  chassis->driveToPoint({4_ft, 6_ft});  //Move back


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
