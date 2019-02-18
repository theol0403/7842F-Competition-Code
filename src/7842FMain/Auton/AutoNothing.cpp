#include "7842FMain/Auton/AutonIncludes.hpp"

void AutonNothing(void* input)
{
}

void AutonTest(void* input)
{
  OdomController* chassis = static_cast<OdomController*>(input);

  // chassis->setSide(side);
  // chassis->setState({0_ft, 0_ft, 0_deg});
  //chassis->driveDistance(1_ft);

  //chassis->turnToAngle(90_deg, makeArc(4, 1));

  chassis->driveToPoint({1_ft, 1_ft});

}


void AutoDraft(void* input)
{
  OdomController* chassis = static_cast<OdomController*>(input);

  //chassis->setState({1_ft, 7_ft, 90_deg}); //Robot is facing cap
  setIntakeMode(intakeModes::loading);

  QLength topShootDistance = 7_ft;
  QLength middleShootDistance = 5_ft;

  chassis->driveToPoint({4.5_ft, 7_ft});                  //Move to ball under cap
  chassis->driveToPoint({1_ft, topShootDistance - 2_ft}); //Move to behind shooting position
  chassis->turn(angleCalc(leftFlag));                         //turn to flag


  AsyncAction shootTopFlag = AsyncAction()
  //.withAction(makeAction( setFlywheelAngle(calculateAngle(middleShootDistance - shootingDelay(getRobotVelocity()))); ), actionTypes::continousBefore)
  .withTrigger(leftFlag, topShootDistance)
  .withMakeAction(setIntakeMode(intakeModes::shootIndexer););

  AsyncAction shootMiddleFlag = AsyncAction()
  .withExclusion(shootTopFlag, exclusionTypes::onlyAfter)
  //.withAction(makeAction( setFlywheelAngle(calculateAngle(middleShootDistance - shootingDelay(getRobotVelocity()))); ), actionModes::continousBefore)
  .withTrigger(leftFlag, middleShootDistance)
  .withMakeAction(setIntakeMode(intakeModes::shootIndexer););

  chassis->driveToPoint({1_ft, 12_ft}, 4, driveSettle, {shootTopFlag, shootMiddleFlag}); // Move forward towards flags and push bottom flag

  chassis->driveToPoint({0.5_ft, 10.5_ft}); //Push Bottom Flag
  chassis->driveToPoint({0.4_ft, 8.6_ft});  //Move back

}
