#include "AutonActions.hpp"

void firstCapMovement(SideController& chassis, QLength y) {
  chassis.setState({startX, y, 90_deg}); // Robot is facing cap

  //this will make the robot intake when it is a distance from the cap
  AsyncAction intake = AsyncAction()
  .withTrigger(makeTrigger(return computeDistanceToPoint(closeCapDrive) < 2_ft;))
  .withMakeAction(robot.intake->setState(IntakeController::intakeBall););

  // Move to ball under cap while intaking
  chassis.driveToPoint({3.9, y}, 1, driveSettle, {intake});
}


void waitForLastMinute(Timer& timer) {
  QTime shootTime {13_s}; //Time to wait before last-minute shooting
  while(timer.millis() < shootTime) pros::delay(20);
}
