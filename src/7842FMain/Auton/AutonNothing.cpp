#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonNothing(void* input)
{
}

void AutonTest(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  chassis.turnToAngle(90_deg);

  //robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBothFlags);

  //chassis.turnToAngle(90_deg);
  //chassis.setState({0_ft, 0_ft, 0_deg});
  //chassis.driveDistance(1_ft);
  //chassis.driveDistance(1_ft);
}

void AutonDraft(void* input)
{
  AutonPasser* passer = static_cast<AutonPasser*>(input);
  auto &[chassis, timer] = *passer;

  // //chassis.setState({1_ft, 7_ft, 90_deg}); //Robot is facing cap
  // robot.intake->setState(IntakeController::loading);
  //
  // QLength topShootDistance = 7_ft;
  // QLength middleShootDistance = 5_ft;
  //
  // chassis.driveToPoint({4.5_ft, 7_ft});                  //Move to ball under cap
  // chassis.driveToPoint({1_ft, topShootDistance - 2_ft}); //Move to behind shooting position
  // chassis.turn(angleCalc(leftFlag));                         //turn to flag
  //
  //
  // AsyncAction shootTopFlag = AsyncAction()
  // //.withAction(makeAction( setFlywheelAngle(calculateAngle(middleShootDistance - shootingDelay(getRobotVelocity()))); ), actionTypes::continousBefore)
  // .withTrigger(leftFlag, topShootDistance)
  // .withMakeAction(robot.intake->setState(IntakeController::shootIndexer););
  //
  // AsyncAction shootMiddleFlag = AsyncAction()
  // .withExclusion(shootTopFlag, exclusionTypes::onlyAfter)
  // //.withAction(makeAction( setFlywheelAngle(calculateAngle(middleShootDistance - shootingDelay(getRobotVelocity()))); ), actionModes::continousBefore)
  // .withTrigger(leftFlag, middleShootDistance)
  // .withMakeAction(robot.intake->setState(IntakeController::shootIndexer););
  //
  // chassis.driveToPoint({1_ft, 12_ft}, 4, driveSettle, {shootTopFlag, shootMiddleFlag}); // Move forward towards flags and push bottom flag
  //
  // chassis.driveToPoint({0.5_ft, 10.5_ft}); //Push Bottom Flag
  // chassis.driveToPoint({0.4_ft, 8.6_ft});  //Move back

}
