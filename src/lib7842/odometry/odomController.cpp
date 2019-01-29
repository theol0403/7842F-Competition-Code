#include "odomController.hpp"

namespace lib7842
{

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
  {
  };


  QAngle OdomController::computeAngleOfPoint(Point point)
  {
    QAngle angle = atan2(point.x.convert(inch) - chassis->state.x.convert(inch), point.y.convert(inch) - chassis->state.y.convert(inch)) * radian;
    return rollAngle180(angle);
  }

  QAngle OdomController::computeAngleToPoint(Point point)
  {
    QAngle angle = computeAngleOfPoint(point) - chassis->state.theta;
    return rollAngle180(angle);
  }

  QLength OdomController::computeDistanceToPoint(Point point)
  {
    return computeDistanceBetweenPoints(chassis->state, point);
  }


  std::function<bool(OdomController*)> OdomController::turnSettle ([](OdomController* that)
  {
    return that->turnPid->isSettled();
  });

  std::function<bool(OdomController*)> OdomController::turnNoSettle ([](OdomController* that)
  {
    return abs(that->turnPid->getError()) < 10;
  });

  std::function<bool(OdomController*)> OdomController::driveSettle ([](OdomController* that)
  {
    return that->distancePid->isSettled() && that->anglePid->isSettled();
  });

  std::function<bool(OdomController*)> OdomController::driveNoSettle ([](OdomController* that)
  {
    return abs(that->distancePid->getError()) < 100; //mm
  });


  void OdomController::turnToAngle(QAngle angle, std::function<bool(OdomController*)> settleFunction)
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
      turnToAngle(angle, turnSettle);
    } else {
      turnToAngle(angle, turnNoSettle);
    }
  }

  void OdomController::turnToPoint(Point point, std::function<bool(OdomController*)> settleFunction)
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

  void OdomController::turnToPoint(Point point, bool settle)
  {
    if(settle) {
      turnToPoint(point, turnSettle);
    } else {
      turnToPoint(point, turnNoSettle);
    }
  }

  void OdomController::turnAngle(QAngle angle, bool settle)
  {
    if(settle) {
      turnToAngle(angle + chassis->state.theta, turnSettle);
    } else {
      turnToAngle(angle + chassis->state.theta, turnNoSettle);
    }
  }


  void OdomController::driveDistanceAtAngle(QLength distance, QAngle angle, std::function<bool(OdomController*)> settleFunction)
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

      double angleVel = chassis->model->maxVelocity * anglePid->calculateErr(angleErr.convert(degree));
      chassis->model->driveVector(distanceVel, angleVel);
      pros::delay(10);
    }
    while(!settleFunction(this));

    chassis->model->driveVector(0, 0);
  }

  void OdomController::driveDistanceAtAngle(QLength distance, QAngle angle, bool settle)
  {
    if(settle) {
      driveDistanceAtAngle(distance, angle, driveSettle);
    } else {
      driveDistanceAtAngle(distance, angle, driveNoSettle);
    }
  }

  void OdomController::driveDistance(QLength distance, bool settle)
  {
    if(settle) {
      driveDistanceAtAngle(distance, chassis->state.theta, driveSettle);
    } else {
      driveDistanceAtAngle(distance, chassis->state.theta, driveNoSettle);
    }
  }


  void OdomController::driveToPoint(Point targetPoint, std::function<bool(OdomController*)> settleFunction)
  {
    distancePid->reset();
    anglePid->reset();

    do
    {
      QAngle angleErr = computeAngleToPoint(targetPoint);

      Point closestPoint = closest(chassis->state, targetPoint);

      //Point closestPoint = computeClosestPoint(chassis->state, targetPoint);
      QLength distanceErr = computeDistanceToPoint(closestPoint);

      QAngle angleToClose = computeAngleToPoint(closestPoint);
      if(shouldDriveBackwards(angleToClose))
      {
        distanceErr = -distanceErr;
        angleErr = rollAngle180(angleErr + 180_deg);
      }

      double distanceVel = chassis->model->maxVelocity * distancePid->calculateErr(distanceErr.convert(millimeter));
      double angleVel = chassis->model->maxVelocity * anglePid->calculateErr(angleErr.convert(degree));
      chassis->model->driveVector(distanceVel, 0);
      pros::delay(10);
    }
    while(!settleFunction(this));

    chassis->model->driveVector(0, 0);
  }

  void OdomController::driveToPoint(Point targetPoint, bool settle)
  {
    if(settle) {
      driveToPoint(targetPoint, driveSettle);
    } else {
      driveToPoint(targetPoint, driveNoSettle);
    }
  }


  void OdomController::driveToPointAndAngle(Point point)
  {
    driveToPoint(point);
    turnToAngle(point.theta);
  }


}
