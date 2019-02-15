#include "7842FMain/Auto/AutoIncludes.hpp"

void AutoNothing(lib7842::autonSides side)
{
}

void AutoTest(lib7842::autonSides side)
{
  bool run = false;
  bool exit = false;
  bool start = false;

  AsyncAction startAction = AsyncAction()
  .withTrigger(makeTrigger(return start;));

  AsyncAction stopAction = AsyncAction()
  .withTrigger(makeTrigger(return exit;));

  AsyncAction testAction = AsyncAction()
  .withAction(makeAction(std::cout << "Continuous Before" << std::endl;), actionTypes::continousBefore)
  .withAction(makeAction(std::cout << "Continuous After" << std::endl;), actionTypes::continousAfter)
  .withAction(makeAction(std::cout << "Once Before" << std::endl;), actionTypes::onceBefore)
  .withAction(makeAction(std::cout << "Once After" << std::endl;))
  .withTrigger(makeTrigger(return run;))
  .withExclusion(startAction, exclusionTypes::onlyAfter)
  .withExclusion(stopAction, exclusionTypes::onlyBefore);

  int count = 0;
  while(true)
  {
    std::cout << "Count is: " << count << std::endl;
    if(count >= 50 && !run)
    {
      run = true;
      std::cout << "TRUE" << std::endl;
    }
    if(count >= 70 && !exit)
    {
      exit = true;
      std::cout << "EXIT" << std::endl;
    }
    if(count >= 20 && !start)
    {
      start = true;
      std::cout << "START" << std::endl;
    }
    startAction.run(nullptr);
    stopAction.run(nullptr);
    testAction.run(nullptr);
    count++;
    pros::delay(200);
  }

}


void AutoDraft(lib7842::autonSides side)
{
  tracker->setState(1_ft, 7_ft, 90_deg); //Robot is facing cap
  setIntakeMode(intakeModes::loading);

  QLength topShootDistance = 7_ft;
  QLength middleShootDistance = 5_ft;

  chassis->driveToPoint({4.5_ft, 7_ft});                  //Move to ball under cap
  chassis->driveToPoint({1_ft, topShootDistance - 2_ft}); //Move to behind shooting position
  chassis->turnToPoint(leftFlag);                         //turn to flag


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
