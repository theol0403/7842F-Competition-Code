#include "odomController.hpp"

namespace lib7842
{

  OdomController::OdomController(
    OdomTracker *ichassis,
    PID *idistancePid,
    PID *ianglePid,
    PID *iturnPid
  )
  :
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

  QAngle OdomController::computeAngleOfPoint(Point point)
  {
    QAngle wantedAngle = atan2(point.x.convert(inch) - chassis->state.x.convert(inch), point.y.convert(inch) - chassis->state.y.convert(inch)) * radian;
    return rollAngle180(wantedAngle);
  }


  QAngle OdomController::computeAngleToPoint(Point point)
  {
    QAngle wantedAngle = computeAngleOfPoint(point) - chassis->state.theta;
    return rollAngle180(wantedAngle);
  }


  void OdomController::turnToAngle(QAngle wantedAngle)
  {
    wantedAngle = rollAngle180(wantedAngle);
    turnPid->reset();
    while(!turnPid->isSettled())
    {
      QAngle angleErr = rollAngle180(wantedAngle - chassis->state.theta);
      double turnVel = 200 * turnPid->calculateErr(angleErr.convert(degree));
      chassis->model->rotate(turnVel);

      pros::delay(10);
    }
    chassis->model->rotate(0);
  }

  void OdomController::turnToPoint(Point point)
  {
    turnPid->reset();
    while(!turnPid->isSettled())
    {
      double turnVel = 200 * turnPid->calculateErr(rollAngle180(computeAngleToPoint(point)).convert(degree));
      chassis->model->rotate(turnVel);
      pros::delay(10);
    }
    chassis->model->rotate(0);
  }

  void OdomController::turnAngle(QAngle wantedAngle)
  {
    turnToAngle(wantedAngle + chassis->state.theta);
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

  void OdomController::driveDistanceToAngle(QLength wantedDistance, QAngle wantedAngle)
  {
    driveDistanceToAngle(wantedDistance, wantedAngle, [](OdomController* that) {
      return !that->distancePid->isSettled() || !that->anglePid->isSettled();
    });
  }


  void OdomController::driveDistanceToAngle(QLength wantedDistance, QAngle wantedAngle, std::function<bool(OdomController*)> settleFunction)
  {
    std::valarray<int32_t> lastTicks = chassis->model->getSensorVals();

    distancePid->reset();
    anglePid->reset();

    while(settleFunction(this))
    {
      std::valarray<int32_t> newTicks = chassis->model->getSensorVals();
      QLength leftDistance = ((newTicks[0] - lastTicks[0]) * chassis->m_mainDegToInch) * inch;
      QLength rightDistance = ((newTicks[1] - lastTicks[1]) * chassis->m_mainDegToInch) * inch;
      QAngle sensorErr = ((leftDistance - rightDistance) / chassis->m_chassisWidth) * radian;

      QLength distanceErr = wantedDistance - (leftDistance + rightDistance) / 2;
      double distanceVel = 200 * distancePid->calculateErr(distanceErr.convert(millimeter));

      QAngle angleErr = rollAngle180(wantedAngle - chassis->state.theta);
      double angleVel = 0;
      if(distanceErr > 5_in)
      {
        angleVel = 200 * anglePid->calculateErr(((sensorErr - angleErr)/2).convert(degree));
      }
      else
      {
        angleVel = 200 * anglePid->calculateErr(sensorErr.convert(degree));
      }

      chassis->model->driveVector(distanceVel, angleVel);
      pros::delay(10); // Run the control loop at 10ms intervals
    }
    chassis->model->driveVector(0, 0);
  }

  void OdomController::driveDistance(QLength wantedDistance, bool settle)
  {
    if(settle)
    {
      driveDistanceToAngle(wantedDistance, chassis->state.theta, [](OdomController* that) {
        return !that->distancePid->isSettled() || !that->anglePid->isSettled();
      });
    }
    else
    {
      driveDistanceToAngle(wantedDistance, chassis->state.theta, [](OdomController* that) {
        return that->distancePid->getError() > 100;
      });
    }
  }


  void OdomController::driveToPoint(Point point)
  {
    int direction = 1;
    QAngle wantedAngle = computeAngleToPoint(point);
    if(wantedAngle.abs() > 90_deg)
    {
      wantedAngle -= 180_deg * sgn(wantedAngle.convert(degree));
      direction = -1;
    }

    turnToAngle(wantedAngle);

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

      double angleVel = 200 * anglePid->calculateErr(angleErr.convert(degree));

      distanceErr = computeDistanceToPoint(point) * direction;
      double distanceVel = 0;
      if(angleErr.abs() < 30_deg)
      {
        distanceVel = 200 * distancePid->calculateErr(distanceErr.convert(millimeter));
      }


      chassis->model->driveVector(distanceVel, angleVel);

      pros::delay(10); // Run the control loop at 10ms intervals
    }
    driveDistanceToAngle(computeDistanceToPoint(point) * direction, wantedAngle);
    chassis->model->driveVector(0, 0);

  }



  void OdomController::driveToPointAndAngle(Point point)
  {
    driveToPoint(point);
    turnToAngle(point.theta);
  }


}
