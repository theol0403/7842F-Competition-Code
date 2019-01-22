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



  QAngle OdomController::computeAngleToPoint(Point point)
  {
    QAngle wantedAngle = (atan2(point.x.convert(inch) - chassis->state.x.convert(inch), point.y.convert(inch) - chassis->state.y.convert(inch)) * radian) - chassis->state.theta;
    if(wantedAngle.abs() > 180_deg)
    {
      wantedAngle -= 360_deg * sgn(wantedAngle.convert(degree));
    }
    return wantedAngle;
  }

  void OdomController::turnAngle(QAngle wantedAngle)
  {
    turnToAngle(wantedAngle + chassis->state.theta);
  }

  void OdomController::turnToAngle(QAngle wantedAngle)
  {
    turnPid->reset();

    while(!turnPid->isSettled())
    {
      double turnVel = turnPid->calculate(wantedAngle.convert(degree), chassis->state.theta.convert(degree));
      chassis->model->rotate(turnVel); //TODO Could be reversed

      pros::delay(10); // Run the control loop at 10ms intervals
    }

    chassis->model->rotate(0);
  }

  void OdomController::turnToPoint(Point point)
  {
    turnToAngle(computeAngleToPoint(point) + chassis->state.theta);
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

  void OdomController::driveDistance(QLength wantedDistance)
  {
    driveDistanceToAngle(wantedDistance, chassis->state.theta);
  }

  void OdomController::driveDistanceToAngle(QLength wantedDistance, QAngle wantedAngle)
  {
    std::valarray<int32_t> lastTicks = chassis->model->getSensorVals();

    distancePid->reset();
    anglePid->reset();

    while(!distancePid->isSettled())
    {
      std::valarray<int32_t> newTicks = chassis->model->getSensorVals();
      QLength newDistance = ((newTicks[0] - lastTicks[0] + newTicks[1] - lastTicks[1]) / 2 * chassis->m_mainDegToInch) * inch;

      QLength distanceErr = wantedDistance - newDistance;
      double distanceVel = 200 * distancePid->calculateErr(distanceErr.convert(millimeter));

      double angleVel = 0;
      if(distanceErr > 5_in)
      {
        angleVel = 200 * anglePid->calculate(wantedAngle.convert(degree), chassis->state.theta.convert(degree));
      }

      chassis->model->driveVector(distanceVel, angleVel);

      pros::delay(10); // Run the control loop at 10ms intervals
    }

    chassis->model->driveVector(0, 0);
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

    while(distanceErr.abs() > 5_in)
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
      double distanceVel;
      if(angleErr.abs() < 30_deg)
      {
        distanceVel = 200 * distancePid->calculateErr(distanceErr.convert(millimeter));
      }


      chassis->model->driveVector(distanceVel, angleVel);

      pros::delay(10); // Run the control loop at 10ms intervals
    }

  }



  void OdomController::driveToPointAndAngle(Point point)
  {
    driveToPoint(point);
    turnToAngle(point.theta);
  }


}
