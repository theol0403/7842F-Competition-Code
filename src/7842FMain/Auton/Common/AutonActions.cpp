#include "7842FMain/AutonIncludes.hpp"

void firstCapMovement(SideController& chassis, QLength y) {
  chassis.setState({startX, y, 90_deg}); // Robot is facing cap

  QPoint drivePoint {3.9_ft, y};

  //this will make the robot intake when it is a distance from the cap
  AsyncAction intake = chassis.makeAsyncAction()
  .withTrigger(drivePoint, 2_ft)
  .withMakeAction(robot.intake->setState(IntakeController::intakeBall););

  // Move to ball under cap while intaking
  chassis.driveToPoint(drivePoint, 1, driveSettle, {intake});
}


void waitForLastMinute(Timer& timer) {
  QTime shootTime {13_s}; //Time to wait before last-minute shooting
  while(timer.getDtFromStart() < shootTime) pros::delay(20);
}



void closeShootFlagsPush(SideController& chassis) {
  chassis.driveToPoint({0.9_ft, 7_ft}, 2); // Move to shooting position

  chassis.turnToPoint(sideFlagShoot); // turn to flag
  robot.shooter->setDistanceToFlag(7_ft);
  robot.shooter->doMacroBlocking(ShootController::shootMacros::shootBoth);

  // Move forward towards flags and push bottom flag
  chassis.driveToPoint({1_ft, 11_ft}, 3, makeSettle(4_in));
}
