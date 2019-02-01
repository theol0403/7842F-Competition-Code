#include "odomController.hpp"

namespace lib7842
{
  using namespace OdomMath;

  OdomController::OdomController(
    OdomTracker *ichassis,
    PID *idistancePid,
    PID *ianglePid,
    PID *iturnPid
  ) :
  chassis(ichassis),
  distancePid(idistancePid),
  anglePid(ianglePid),
  turnPid(iturnPid)
  {};


  bool OdomController::turnSettle(OdomController* that) {
    return that->turnPid->isSettled();
  }

  bool OdomController::turnNoSettle(OdomController* that) {
    return abs(that->turnPid->getError()) < 10;
  }

  bool OdomController::driveSettle(OdomController* that) {
    return that->distancePid->isSettled() && that->anglePid->isSettled();
  }

  bool OdomController::driveNoSettle(OdomController* that) {
    return abs(that->distancePid->getError()) < 10; //mm
  }

  std::function<bool(OdomController*)> OdomController::createTurnSettle(QAngle threshold)
  {
    return [=](OdomController* that){ return abs(that->turnPid->getError()) < threshold.convert(degree); };
  }

  std::function<bool(OdomController*)> OdomController::createDriveSettle(QLength threshold)
  {
    return [=](OdomController* that){ return abs(that->distancePid->getError()) < threshold.convert(millimeter); };
  }


  QAngle OdomController::computeAngleToPoint(qPoint point)
  {
    QAngle angle = (atan2(point.x.convert(inch) - chassis->state.x.convert(inch), point.y.convert(inch) - chassis->state.y.convert(inch)) * radian) - chassis->state.theta;
    return rollAngle180(angle);
}

  QLength OdomController::computeDistanceToPoint(qPoint point)
  {
    return computeDistanceBetweenPoints(chassis->state, point);
  }

  void OdomController::normalizeDrive(double &distanceVel, double &angleVel)
  {
    double maxMag = std::max(fabs(angleVel), fabs(distanceVel));
    if(maxMag > chassis->model->maxVelocity) {
      distanceVel = (distanceVel / maxMag) * chassis->model->maxVelocity;
      angleVel = (angleVel / maxMag) * chassis->model->maxVelocity;
    }
  }







  void OdomController::turnToAngleSettle(QAngle angle, std::function<bool(OdomController*)> settleFunction)
  {
    angle = rollAngle180(angle);
    turnPid->reset();
    do {
      QAngle angleErr = rollAngle180(angle - chassis->state.theta);
      double turnVel = chassis->model->maxVelocity * turnPid->calculateErr(angleErr.convert(degree));
      chassis->model->rotate(turnVel);
      pros::delay(10);
    }
    while(!settleFunction(this));

    chassis->model->rotate(0);
  }

  void OdomController::turnToAngle(QAngle angle, bool settle)
  {
    if(settle) {
      turnToAngleSettle(angle, turnSettle);
    } else {
      turnToAngleSettle(angle, turnNoSettle);
    }
  }

  void OdomController::turnAngle(QAngle angle, bool settle)
  {
    if(settle) {
      turnToAngleSettle(angle + chassis->state.theta, turnSettle);
    } else {
      turnToAngleSettle(angle + chassis->state.theta, turnNoSettle);
    }
  }

  void OdomController::turnToPointSettle(qPoint point, std::function<bool(OdomController*)> settleFunction)
  {
    turnPid->reset();
    do
    {
      double turnVel = chassis->model->maxVelocity * turnPid->calculateErr(rollAngle180(computeAngleToPoint(point)).convert(degree));
      chassis->model->rotate(turnVel);
      pros::delay(10);
    }
    while(!settleFunction(this));

    chassis->model->rotate(0);
  }

  void OdomController::turnToPoint(qPoint point, bool settle)
  {
    if(settle) {
      turnToPointSettle(point, turnSettle);
    } else {
      turnToPointSettle(point, turnNoSettle);
    }
  }







  void OdomController::driveDistanceAtAngleSettle(QLength distance, QAngle angle, std::function<bool(OdomController*)> settleFunction, double turnScale, bool reset)
  {
    //distance = distance/2; //Idk
    angle = rollAngle180(angle);

    if(reset)
    {
      distancePid->reset();
      anglePid->reset();
    }

    std::valarray<int32_t> lastTicks = chassis->model->getSensorVals();
    do
    {
      //std::cout << "DistanceAtAngle" << std::endl;
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

  void OdomController::driveDistanceAtAngle(QLength distance, QAngle angle, bool settle, double turnScale)
  {
    if(settle) {
      driveDistanceAtAngleSettle(distance, angle, driveSettle, turnScale);
    } else {
      driveDistanceAtAngleSettle(distance, angle, driveNoSettle, turnScale);
    }
  }

  void OdomController::driveDistance(QLength distance, bool settle)
  {
    if(settle) {
      driveDistanceAtAngleSettle(distance, chassis->state.theta, driveSettle);
    } else {
      driveDistanceAtAngleSettle(distance, chassis->state.theta, driveNoSettle);
    }
  }








  void OdomController::driveToPoint1Settle(qPoint targetPoint, std::function<bool(OdomController*)> settleFunction, double turnScale)
  {
    distancePid->reset();
    anglePid->reset();

    do
    {

      QAngle angleErr = computeAngleToPoint(targetPoint);
      QLength distanceToTarget = computeDistanceToPoint(targetPoint);
      if(distanceToTarget.abs() < 4_in)
      {
        angleErr = 0_deg;
      }

      if(angleErr.abs() > 90_deg)
      {
        angleErr -= 180_deg;
        angleErr = rollAngle180(angleErr) * sgn(angleErr.convert(degree));
      }

      qPoint closestPoint = closest(chassis->state, targetPoint);

      QAngle angleToClose = computeAngleToPoint(closestPoint);
      //if(angleToClose.abs() < 0.5_deg || std::isnan(angleToClose.convert(degree))) { angleToClose = 0_deg; }

      QLength distanceErr = computeDistanceToPoint(closestPoint);

      if(angleToClose.abs() >= 90_deg)
      {
        distanceErr = -distanceErr;
      }

      double angleVel = chassis->model->maxVelocity * anglePid->calculateErr(angleErr.convert(degree) / turnScale) * turnScale;
      double distanceVel = chassis->model->maxVelocity * distancePid->calculateErr(distanceErr.convert(millimeter));

      normalizeDrive(distanceVel, angleVel);
      chassis->model->driveVector(distanceVel, angleVel);
      pros::delay(10);
    }
    while(!settleFunction(this));

    chassis->model->driveVector(0, 0);
  }

  void OdomController::driveToPoint1(qPoint targetPoint, bool settle, double turnScale)
  {
    if(settle) {
      driveToPoint1Settle(targetPoint, driveSettle, turnScale);
    } else {
      driveToPoint1Settle(targetPoint, driveNoSettle, turnScale);
    }
  }


  void OdomController::driveToPoint2Settle(qPoint targetPoint, std::function<bool(OdomController*)> settleFunction, double turnScale)
  {
    distancePid->reset();
    anglePid->reset();

    QAngle angleErr;
    QLength distanceErr;

    do
    {
      angleErr = computeAngleToPoint(targetPoint);
      distanceErr = computeDistanceToPoint(targetPoint);

      if(angleErr.abs() > 90_deg)
      {
        angleErr -= 180_deg;
        angleErr = rollAngle180(angleErr) * sgn(angleErr.convert(degree));
        distanceErr = -distanceErr;
      }

      double angleVel = chassis->model->maxVelocity * anglePid->calculateErr(angleErr.convert(degree) / turnScale) * turnScale;
      double distanceVel = chassis->model->maxVelocity * distancePid->calculateErr(distanceErr.convert(millimeter));

      normalizeDrive(distanceVel, angleVel);
      chassis->model->driveVector(distanceVel, angleVel);
      pros::delay(100);
    }
    while(distanceErr.abs() > 4_in);

    driveDistanceAtAngleSettle(distanceErr/2, chassis->state.theta, settleFunction, turnScale, false);

    chassis->model->driveVector(0, 0);
  }

  void OdomController::driveToPoint2(qPoint targetPoint, bool settle, double turnScale)
  {
    if(settle) {
      driveToPoint2Settle(targetPoint, driveSettle, turnScale);
    } else {
      driveToPoint2Settle(targetPoint, driveNoSettle, turnScale);
    }
  }








  void OdomController::driveForTime(double vel, int time)
  {
    chassis->model->driveVector(vel, 0);
    pros::delay(time);
    chassis->model->driveVector(0, 0);
  }

  void OdomController::driveForTimeAtAngle(double vel, QAngle angle, int time, double turnScale)
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
