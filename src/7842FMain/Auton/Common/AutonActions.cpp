#include "7842FMain/Auton/AutonIncludes.hpp"

void firstCapMovement(SideController& chassis, QLength y) {
  chassis.setState({startX, y, 90_deg}); // Robot is facing cap

  QPoint drivePoint {3.8_ft, y};

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
