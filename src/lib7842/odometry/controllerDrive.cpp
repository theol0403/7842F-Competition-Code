#include "controller.hpp"

namespace lib7842
{

  void OdomController::driveDistanceAtAngle(QLength distance, angleCalc_t turnCalc, double turnScale, settleFunc_t settleFunc, AsyncActionList actions)
  {
    resetEmergencyAbort();
    std::valarray<int32_t> lastTicks = tracker->model->getSensorVals();

    do
    {
      std::valarray<int32_t> newTicks = tracker->model->getSensorVals();
      QLength leftDistance = ((newTicks[0] - lastTicks[0]) * tracker->m_mainDegToInch) * inch;
      QLength rightDistance = ((newTicks[1] - lastTicks[1]) * tracker->m_mainDegToInch) * inch;

      m_distanceErr = distance - ((leftDistance + rightDistance) / 2);
      m_angleErr = turnCalc(this);

      double distanceVel = distancePid->calculateErr(m_distanceErr.convert(millimeter));
      double angleVel = anglePid->calculateErr(m_angleErr.convert(degree));

      driveVector(distanceVel, angleVel * turnScale);
      runActions(actions);
      pros::delay(10);
    }
    while(!settleFunc(this));

    driveVector(0, 0);
  }


  void OdomController::driveDistance(QLength distance, settleFunc_t settleFunc, AsyncActionList actions)
  {
    driveDistanceAtAngle(distance, angleCalc(getTheta()), 1, settleFunc, actions);
  }


  void OdomController::driveForTime(int time, double vel, AsyncActionList actions)
  {
    while(time > 0) {
      driveVector(vel, 0);
      time -= 10;
      runActions(actions);
      pros::delay(10);
    }
    driveVector(0, 0);
  }


  void OdomController::driveForTimeAtAngle(int time, double vel, angleCalc_t turnCalc, double turnScale, AsyncActionList actions)
  {
    while(time > 0) {
      m_angleErr = turnCalc(this);
      double angleVel = anglePid->calculateErr(m_angleErr.convert(degree));
      driveVector(vel, angleVel * turnScale);
      time -= 10;
      runActions(actions);
      pros::delay(10);
    }
    driveVector(0, 0);
  }


  void OdomController::allignToAngle(QAngle angle, double vel, double velThresh)
  {
    angle = rollAngle180(angle);
    angle = setSide(angle);
    turnToAngle(angle);
    resetVelocityMax();
    tracker->model->forward(vel);
    while(filterVelocity() > velThresh) { pros::delay(10); }
    tracker->setTheta(angle);
  }


}
