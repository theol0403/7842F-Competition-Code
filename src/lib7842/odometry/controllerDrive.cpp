#include "controller.hpp"

namespace lib7842 {

void OdomController::driveDistanceAtAngle(QLength distance, angleCalc_t turnCalc, double turnScale,
                                          settleFunc_t settleFunc, AsyncActionList actions) {
  resetPid();
  std::valarray<int32_t> lastTicks = tracker->model->getSensorVals();

  do {
    std::valarray<int32_t> newTicks = tracker->model->getSensorVals();
    QLength leftDistance = ((newTicks[0] - lastTicks[0]) * tracker->m_degToInch) * inch;
    QLength rightDistance = ((newTicks[1] - lastTicks[1]) * tracker->m_degToInch) * inch;

    distanceErr = distance - ((leftDistance + rightDistance) / 2);
    angleErr = turnCalc(this);

    double distanceVel = distancePid->step(-distanceErr.convert(millimeter));
    double angleVel = anglePid->step(-angleErr.convert(degree));

    driveVector(distanceVel, angleVel * turnScale);
    runActions(actions);
    pros::delay(10);
  } while (!settleFunc(this));

  driveVector(0, 0);
}

void OdomController::driveDistance(QLength distance, settleFunc_t settleFunc,
                                   AsyncActionList actions) {
  driveDistanceAtAngle(distance, angleCalc(tracker->state.theta), 1, settleFunc, actions);
}

void OdomController::driveForTime(int time, double vel, AsyncActionList actions) {
  while (time > 0) {
    driveVector(vel, 0, true);
    time -= 10;
    runActions(actions);
    pros::delay(10);
  }
  driveVector(0, 0, true);
}

void OdomController::driveForTimeAtAngle(int time, double vel, angleCalc_t turnCalc,
                                         double turnScale, AsyncActionList actions) {
  while (time > 0) {
    angleErr = turnCalc(this);
    double angleVel = anglePid->step(-angleErr.convert(degree));
    driveVector(vel, angleVel * turnScale);
    time -= 10;
    runActions(actions);
    pros::delay(10);
  }
  driveVector(0, 0);
}

void OdomController::allignToAngle(QAngle angle, double vel, double velThresh) {
  angle = rollAngle180(angle);
  turnToAngle(angle);
  while (!checkAbort(velThresh, 2_s)) {
    tracker->model->forward(vel);
    pros::delay(10);
  }
  tracker->setTheta(angle);
}

} // namespace lib7842
