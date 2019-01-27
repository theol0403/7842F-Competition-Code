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


  QAngle OdomController::rollAngle360(QAngle angle)
  {
    return angle - 360.0_deg * std::floor(angle.convert(degree) * (1.0 / 360.0));
  }

  QAngle OdomController::rollAngle180(QAngle angle)
  {
    return angle - 360.0_deg * std::floor((angle.convert(degree) + 180.0) * (1.0 / 360.0));
  }

  bool OdomController::shouldDriveBackwards(QAngle angle)
  {
    if(rollAngle180(angle).abs() > 90_deg) { return true; }
    else { return false; }
  }


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


  QLength OdomController::computeDistanceBetweenPoints(Point firstPoint, Point secondPoint)
  {
    const QLength xDiff = secondPoint.x - firstPoint.x;
    const QLength yDiff = secondPoint.y - firstPoint.y;
    return std::sqrt(std::pow(xDiff.convert(inch), 2) + std::pow(yDiff.convert(inch), 2)) * inch;
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
    return that->turnPid->getError() < 10;
  });

  std::function<bool(OdomController*)> OdomController::driveSettle ([](OdomController* that)
  {
    return that->distancePid->isSettled() && that->anglePid->isSettled();
  });

  std::function<bool(OdomController*)> OdomController::driveNoSettle ([](OdomController* that)
  {
    return that->distancePid->getError() < 200;
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











  void OdomController::driveToPoint(Point point)
  {
    int direction = 1;
    QAngle angle = computeAngleToPoint(point);
    if(angle.abs() > 90_deg)
    {
      angle -= 180_deg * sgn(angle.convert(degree));
      direction = -1;
    }

    turnToAngle(angle);

    distancePid->reset();
    anglePid->reset();

    QLength distanceErr = computeDistanceToPoint(point) * direction;

    while(distanceErr.abs() > 4_in)
    {
      QAngle angleErr = computeAngleToPoint(point);
      if(angleErr.abs() > 90_deg)
      {
        angleErr -= 180_deg * sgn(angleErr.convert(degree));
        direction = -1;
      }
      else
      {
        direction = 1;
      }

      double angleVel = chassis->model->maxVelocity * anglePid->calculateErr(angleErr.convert(degree));

      distanceErr = computeDistanceToPoint(point) * direction;
      double distanceVel = 0;
      if(angleErr.abs() < 30_deg)
      {
        distanceVel = chassis->model->maxVelocity * distancePid->calculateErr(distanceErr.convert(millimeter));
      }


      chassis->model->driveVector(distanceVel, angleVel);

      pros::delay(10); // Run the control loop at 10ms intervals
    }
    driveDistanceAtAngle(computeDistanceToPoint(point) * direction, angle);
    chassis->model->driveVector(0, 0);

  }



  void OdomController::driveToPointAndAngle(Point point)
  {
    driveToPoint(point);
    turnToAngle(point.theta);
  }


}
