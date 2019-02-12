#include "7842FMain/Auto/AutoIncludes.hpp"

void AutoNothing(lib7842::autonSides side)
{
  tracker->setState(1_ft, 7_ft, 90_deg); //Robot is facing cap
  setIntakeMode(intakeModes::loading);

  QLength topShootDistance = 7_ft;
  QLength middleShootDistance = 5_ft;
  setFlywheelRPM(calculateRPM(topShootDistance), topFlagHeight); //speed to shoot top flag

  chassis->driveToPoint({4.5_ft, 7_ft});                  //Move to ball under cap
  chassis->driveToPoint({1_ft, topShootDistance - 2_ft}); //Move to behind shooting position
  chassis->turnToPoint(leftFlag);                         //turn to flag

  AsyncAction shootTopFlag = AsyncAction::create()
  .withBeforeAction( setFlywheelRPM(calculateRPM(topShootDistance - shootingDelay(robotVelocity))); )
  .withTrigger( return calculateDistanceToPoint(leftFlag) < topShootDistance; )
  .withAction( setIntakeMode(intakeModes::shootIndexer); )
  .build();

  AsyncAction shootMiddleFlag = AsyncAction::create()
  .withBeforeAction( setFlywheelRPM(calculateRPM(middleShootDistance - shootingDelay(robotVelocity))); )
  .withTrigger( return calculateDistanceToPoint(leftFlag) < middleShootDistance; )
  .withAction( setIntakeMode(intakeModes::shootIndexer); )
  .onlyAfter(shootTopFlag)
  .build();

  chassis->driveToPoint({1_ft, 12_ft}, {shootTopFlag, shootMiddleFlag}); // Move forward towards flags and push bottom flag

  chassis->driveToPoint({0.5_ft, 10.5_ft}); //Push Bottom Flag
  chassis->driveToPoint({0.4_ft, 8.6_ft});  //Move back

}

void AutoTest(lib7842::autonSides side)
{
  // chassis->driveDistance(1_ft);
  // chassis->driveToPoint({2_ft, 1_ft}, 3, makeSettle(1_in));
  //chassis->driveToPoint({0_in, 1_ft}, 3, driveSettle);
  chassis->drivePath({{0_in, 2_ft}, {2_ft, 2_ft}, {0_ft, 0_ft}}, 4, makeSettle(4_in));

  asyncAction_t shootFlag{
    {makeTrigger(leftFlag, 10_in, 5_deg)},
    {makeAction(setIntakeMode(intakeModes::shootIndexer);)}};
    chassis->driveDistanceAtAngle(10_in, 0_deg, 1, driveSettle, {shootFlag});
  }
