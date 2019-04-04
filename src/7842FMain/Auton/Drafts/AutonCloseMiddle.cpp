#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonCloseMiddle(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
auto &[chassis, timer] = *passer;

  chassis.setState({ 1_ft, 7_ft, 90_deg }); // Robot is facing cap

  AsyncAction intake = AsyncAction()
  .withTrigger(makeTrigger(return computeDistanceToPoint(closeCapDrive) < 2_ft;))
  .withMakeAction(robot.intake->setState(IntakeController::intakeBall););

  chassis.driveToPoint(closeCapDrive, 1, makeSettle(3_in), {intake}); // Move to ball under cap
  chassis.driveToPoint({1_ft, 7.2_ft}, 1, makeSettle(2_in)); // Move to behind shooting position -- was 6.7

  chassis.turnToPoint(sideFlagShoot); // turn to flag

  robot.shooter->setTarget(0);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTarget);
  robot.shooter->setTarget(20);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTarget);

  chassis.driveToPointSimple(closeFlatCap, makeSettle(3_in));
  robot.arm->setState(ArmController::down);
  chassis.driveDistance(-4_in);

  chassis.turnToPoint(middleFlagPost);
  robot.shooter->setTarget(0);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTarget);
  robot.shooter->setTarget(20);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootTarget);

  //   setIntakeMode(intakeModes::loading);
  //
  //   if(side == autonSides::red)
  //   {
  //     setFlywheelRPM(2800);
  //     tracker->setState(1_ft, 7_ft, 90_deg);
  //
  //
  //     chassis.driveToPoint({4.5_ft, 7_ft}); //Move to ball under cap
  //     chassis.driveToPoint({1_ft, 7_ft}); //Move to shooting position
  //
  //     chassis.turnToPoint(leftFlag); //Shoot
  //     setIntakeMode(intakeModes::shootIndexer);
  //     pros::delay(200);
  //
  //     chassis.driveToPoint({0.5_ft, 8.8_ft}); // Move forward to second shooting position
  //
  //     chassis.turnToPoint(leftFlag); //Shoot
  //     setIntakeMode(intakeModes::shootIndexer);
  //     pros::delay(200);
  //
  //     chassis.driveToPoint({0.5_ft, 10.5_ft}); //Push Bottom Flag
  //     chassis.driveToPoint({0.4_ft, 8.6_ft}); //Move back
  //
  //     chassis.turnToAngle(-90_deg); //Turn to cap
  //     tracker->setState(1_ft, 9_ft, -90_deg);
  //
  //     //setFlywheelRPM(3000);
  //     chassis.driveToPoint({3.9_ft, 9_ft}); //Flip Cap
  //
  //     // chassis.turnToAngle(45_deg); //Turn to flag
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
  //     chassis.driveToPoint({FX 4.5_ft, 7_ft}); //Move to ball under cap
  //     chassis.driveToPoint({FX 1.1_ft, 7_ft}); //Move to shooting position
  //
  //     chassis.turnToPoint(rightFlag); //Shoot first ball
  //     setIntakeMode(intakeModes::shootIndexer);
  //     pros::delay(200);
  //
  //     tracker->setX(FX 1_ft);
  //     chassis.driveToPoint({FX 0.7_ft, 10_ft}); // Move forward to second shooting position
  //
  //     chassis.turnToPoint(rightFlag); //Shoot second ball
  //     setIntakeMode(intakeModes::shootIndexer);
  //     pros::delay(200);
  //
  //     chassis.driveToPoint({FX 0.6_ft, 11.5_ft}); //Push Bottom Flag
  //     chassis.driveToPoint({FX 1_ft, 9.5_ft}); //Move back
  //
  //     chassis.turnToAngle(FA -90_deg); //Turn to cap
  //     tracker->setState(FX 1_ft, 9_ft, FA -90_deg); //Set state before cap
  //
  //     setFlywheelRPM(3000);
  //     chassis.driveToPoint({FX 3.9_ft, 9_ft}); //Flip Cap
  //
  //     chassis.turnToAngle(FA 45_deg); //Turn to flag
  //
  //     setIntakeMode(intakeModes::shootIndexer);
  //     pros::delay(200);
  //   }
}
