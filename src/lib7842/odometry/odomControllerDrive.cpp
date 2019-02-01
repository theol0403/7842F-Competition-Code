#include "odomController.hpp"

namespace lib7842
{

  void OdomController::driveDistanceAtAngleSettle(QLength distance, QAngle angle, double turnScale, std::function<bool(OdomController*)> settleFunction, bool reset)
  {
    //distance = distance/2; //Idk
    angle = rollAngle180(angle);

    if(reset) {
      distancePid->reset();
      anglePid->reset();
    }

    std::valarray<int32_t> lastTicks = chassis->model->getSensorVals();

    do
    {
      std::valarray<int32_t> newTicks = chassis->model->getSensorVals();
      QLength leftDistance = ((newTicks[0] - lastTicks[0]) * chassis->m_mainDegToInch) * inch;
      QLength rightDistance = ((newTicks[1] - lastTicks[1]) * chassis->m_mainDegToInch) * inch;

      QLength distanceErr = distance - ((leftDistance + rightDistance) / 2);
      double distanceVel = chassis->model->maxVelocity * distancePid->calculateErr(distanceErr.convert(millimeter));

      QAngle angleErr = rollAngle180(angle - chassis->state.theta);
      double angleVel = chassis->model->maxVelocity * anglePid->calculateErr(angleErr.convert(degree) / turnScale) * turnScale;

      normalizeDrive(distanceVel, angleVel);
      chassis->model->driveVector(distanceVel, angleVel);
      pros::delay(10);
    }
    while(!settleFunction(this));

    chassis->model->driveVector(0, 0);
  }


  void OdomController::driveDistanceAtAngle(QLength distance, QAngle angle, double turnScale, bool settle)
  {
    if(settle) {
      driveDistanceAtAngleSettle(distance, angle, turnScale, driveSettle);
    } else {
      driveDistanceAtAngleSettle(distance, angle, turnScale, driveNoSettle);
    }
  }

  void OdomController::driveDistance(QLength distance, bool settle)
  {
    if(settle) {
      driveDistanceAtAngleSettle(distance, chassis->state.theta, 1, driveSettle);
    } else {
      driveDistanceAtAngleSettle(distance, chassis->state.theta, 1, driveNoSettle);
    }
  }



  void OdomController::driveForTime(int time, double vel)
  {
    chassis->model->driveVector(vel, 0);
    pros::delay(time);
    chassis->model->driveVector(0, 0);
  }


  void OdomController::driveForTimeAtAngle(int time, double vel, QAngle angle, double turnScale)
  {
    while(time > 0) {
      QAngle angleErr = rollAngle180(angle - chassis->state.theta);
      double angleVel = chassis->model->maxVelocity * anglePid->calculateErr(angleErr.convert(degree) / turnScale) * turnScale;
      chassis->model->driveVector(vel, angleVel);
      time -= 10;
      pros::delay(10);
    }
    chassis->model->driveVector(0, 0);
  }


}
