#include "controller.hpp"

namespace lib7842
{

  void OdomController::driveDistanceAtAngle(QLength distance, QAngle angle, double turnScale, settleFunc_t settleFunc)
  {
    resetEmergencyAbort();
    //distance = distance/2; //Idk
    angle = rollAngle180(angle);
    std::valarray<int32_t> lastTicks = tracker->model->getSensorVals();

    do
    {
      std::valarray<int32_t> newTicks = tracker->model->getSensorVals();
      QLength leftDistance = ((newTicks[0] - lastTicks[0]) * tracker->m_mainDegToInch) * inch;
      QLength rightDistance = ((newTicks[1] - lastTicks[1]) * tracker->m_mainDegToInch) * inch;

      m_distanceErr = distance - ((leftDistance + rightDistance) / 2);
      m_angleErr = rollAngle180(angle - tracker->state.theta);

      double distanceVel = distancePid->calculateErr(m_distanceErr.convert(millimeter));
      double angleVel = anglePid->calculateErr(m_angleErr.convert(degree));

      driveVector(distanceVel, angleVel * turnScale);
      pros::delay(10);
    }
    while(!settleFunc(this));

    driveVector(0, 0);
  }


  void OdomController::driveDistance(QLength distance, settleFunc_t settleFunc)
  {
    driveDistanceAtAngle(distance, tracker->state.theta, 1, settleFunc);
  }


  void OdomController::driveForTime(int time, double vel)
  {
    driveVector(vel, 0);
    pros::delay(time);
    driveVector(0, 0);
  }


  void OdomController::driveForTimeAtAngle(int time, double vel, QAngle angle, double turnScale)
  {
    while(time > 0) {
      m_angleErr = rollAngle180(angle - tracker->state.theta);
      double angleVel = anglePid->calculateErr(m_angleErr.convert(degree));
      driveVector(vel, angleVel * turnScale);
      time -= 10;
      pros::delay(10);
    }
    driveVector(0, 0);
  }


  void OdomController::allignToAngle(QAngle angle, double vel, double velThresh)
  {
    angle = rollAngle180(angle);
    turnToAngle(angle);
    resetVelocityMax();
    tracker->model->forward(vel);
    while(filterVelocity() > velThresh) { pros::delay(10); }
    tracker->setTheta(angle);
  }


}
