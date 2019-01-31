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
    return abs(that->distancePid->getError()) < 100; //mm
  }

  std::function<bool(OdomController*)> OdomController::createTurnSettle(QAngle threshold)
  {
    return [=](OdomController* that){ return abs(that->turnPid->getError()) < threshold.convert(degree); };
  }

  std::function<bool(OdomController*)> OdomController::createDriveSettle(QLength threshold)
  {
    return [=](OdomController* that){ return abs(that->distancePid->getError()) < threshold.convert(millimeter); };
  }


  QAngle OdomController::computeAngleOfPoint(qPoint point)
  {
    QAngle angle = atan2(point.x.convert(millimeter) - chassis->state.x.convert(millimeter), point.y.convert(millimeter) - chassis->state.y.convert(millimeter)) * radian;
    return rollAngle180(angle);
  }

  QAngle OdomController::computeAngleToPoint(qPoint point)
  {
    QAngle angle = computeAngleOfPoint(point) - chassis->state.theta;
    return rollAngle180(angle);
  }

  QLength OdomController::computeDistanceToPoint(qPoint point)
  {
    return computeDistanceBetweenPoints(chassis->state, point);
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


  void OdomController::driveDistanceAtAngleSettle(QLength distance, QAngle angle, std::function<bool(OdomController*)> settleFunction, double turnScale)
  {
    angle = rollAngle180(angle);
    distancePid->reset();
    anglePid->reset();
    std::valarray<int32_t> lastTicks = chassis->model->getSensorVals();
    do
    {
      std::valarray<int32_t> newTicks = chassis->model->getSensorVals();
      QLength leftDistance = ((newTicks[0] - lastTicks[0]) * chassis->m_mainDegToInch) * inch;
      QLength rightDistance = ((newTicks[1] - lastTicks[1]) * chassis->m_mainDegToInch) * inch;

      QLength distanceErr = distance - (leftDistance + rightDistance) / 2;
      double distanceVel = chassis->model->maxVelocity * distancePid->calculateErr(distanceErr.convert(millimeter));

      QAngle angleErr = rollAngle180(angle - chassis->state.theta);
      double angleVel = chassis->model->maxVelocity * anglePid->calculateErr(angleErr.convert(degree) / turnScale) * turnScale;

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


  void OdomController::driveToPointSettle(qPoint targetPoint, std::function<bool(OdomController*)> settleFunction, double turnScale)
  {
    distancePid->reset();
    anglePid->reset();

    do
    {
      qPoint closestPoint = closest(chassis->state, targetPoint);
      QLength distanceErr = computeDistanceToPoint(closestPoint);

      QAngle angleErr = 0_deg;
      QLength distanceToTarget = computeDistanceToPoint(targetPoint);
      if(distanceToTarget.abs() < 4_in)
      {
        angleErr = 0_deg;
      }
      else
      {
        angleErr = computeAngleToPoint(targetPoint);
      }

      std::cout << "Distance To Close: " << distanceErr.convert(inch) << std::endl;
      std::cout << "Angle To Target: " << angleErr.convert(degree) << std::endl;

      if(angleErr.abs() > 90_deg)
      {
        angleErr = angleErr - 180_deg * sgn(angleErr.convert(degree));
        angleErr = rollAngle180(angleErr);
      }

      std::cout << "Rolled To Target: " << angleErr.convert(degree) << std::endl;

      QAngle angleToClose = computeAngleToPoint(closestPoint);
      if(angleToClose.abs() < 1_deg || std::isnan(angleToClose.convert(degree)))
      {
        angleToClose = 0_deg;
      }
      std::cout << "Angle to Close: " << angleToClose.convert(degree) << std::endl;
      if(angleToClose.abs() > 90_deg)
      {
        distanceErr = -distanceErr;
        std::cout << "Backwards" << std::endl;
      }

      double angleVel = chassis->model->maxVelocity * anglePid->calculateErr(angleErr.convert(degree) / turnScale) * turnScale;
      double distanceVel = chassis->model->maxVelocity * distancePid->calculateErr(distanceErr.convert(millimeter));

      double maxMag = std::max(fabs(angleVel), fabs(distanceVel));
      if(maxMag > chassis->model->maxVelocity) {
        maxMag = (maxMag / maxMag) * chassis->model->maxVelocity * sgn(maxMag);
        distanceVel = (distanceVel / maxMag) * chassis->model->maxVelocity * sgn(distanceVel);
      }

      chassis->model->driveVector(distanceVel, angleVel);
      pros::delay(100);
    }
    while(!settleFunction(this));

    chassis->model->driveVector(0, 0);
  }

  void OdomController::driveToPoint(qPoint targetPoint, bool settle, double turnScale)
  {
    if(settle) {
      driveToPointSettle(targetPoint, driveSettle, turnScale);
    } else {
      driveToPointSettle(targetPoint, driveNoSettle, turnScale);
    }
  }


  void OdomController::driveToPointAndAngle(qPoint point)
  {
    driveToPoint(point);
    turnToAngle(point.theta);
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
