#include "odomController.hpp"

namespace lib7842
{

  OdomController::OdomController(
    OdomTracker *chassis,
    PID *distancePid,
    PID *anglePid,
    PID *turnPid
  )
  :
  m_chassis(chassis),
  m_distancePid(distancePid),
  m_anglePid(anglePid),
  m_turnPid(turnPid)
  {
  };

  QAngle OdomController::computeAngleToPoint(Point point)
  {
    return atan2(point.x.convert(inch) - m_chassis->state.x.convert(inch), point.y.convert(inch) - m_chassis->state.y.convert(inch)) * radian - m_chassis->state.theta;
  }

  void OdomController::turnAngle(QAngle wantedAngle)
  {
    turnToAngle(wantedAngle + m_chassis->state.theta);
  }

  void OdomController::turnToAngle(QAngle wantedAngle)
  {
    m_turnPid->reset();

    while(!m_turnPid->isSettled())
    {
      double turnVel = 200 * m_turnPid->calculate(wantedAngle.convert(degree), m_chassis->state.theta.convert(degree));
      m_chassis->model->rotate(turnVel); //TODO Could be reversed

      pros::delay(10); // Run the control loop at 10ms intervals
    }

    m_chassis->model->rotate(0);
  }

  void OdomController::turnToPoint(Point point)
  {
    turnToAngle(computeAngleToPoint(point) + m_chassis->state.theta);
  }


  QLength OdomController::computeDistanceToPoint(Point point)
  {
    return computeDistanceBetweenPoints(m_chassis->state, point);
  }

  QLength OdomController::computeDistanceBetweenPoints(Point firstPoint, Point secondPoint)
  {
    const QLength xDiff = secondPoint.x - firstPoint.x;
    const QLength yDiff = secondPoint.y - firstPoint.y;
    return std::sqrt(std::pow(xDiff.convert(inch), 2) + std::pow(yDiff.convert(inch), 2)) * inch;
  }

  void OdomController::driveDistance(QLength wantedDistance)
  {
    driveDistanceToAngle(wantedDistance, m_chassis->state.theta);
  }


  void OdomController::driveDistanceToAngle(QLength wantedDistance, QAngle wantedAngle)
  {
    std::valarray<int32_t> lastTicks = m_chassis->model->getSensorVals();

    m_distancePid->reset();
    m_anglePid->reset();

    while(!m_distancePid->isSettled())
    {
      std::valarray<int32_t> newTicks = m_chassis->model->getSensorVals();
      QLength newDistance = ((newTicks[0] - lastTicks[0] + newTicks[1] - lastTicks[1]) / 2 * m_chassis->m_mainDegToInch) * inch;

      QLength distanceErr = wantedDistance - newDistance;
      double distanceVel = 200 * m_distancePid->calculateErr(distanceErr.convert(millimeter));

      double angleVel = 0;
      if(distanceErr > 5_in)
      {
        angleVel = 200 * m_anglePid->calculate(wantedAngle.convert(degree), m_chassis->state.theta.convert(degree));
      }

      m_chassis->model->driveVector(distanceVel, angleVel);

      pros::delay(10); // Run the control loop at 10ms intervals
    }

    m_chassis->model->driveVector(0, 0);
  }


  void OdomController::driveToPoint(Point point)
  {
    turnToPoint(point);

    m_distancePid->reset();
    m_anglePid->reset();

    QLength distanceErr = computeDistanceToPoint(point);

    while(distanceErr > 5_in)
    {
      QAngle angleErr = computeAngleToPoint(point);
      double angleVel = 200 * m_anglePid->calculateErr(angleErr.convert(degree));


      distanceErr = computeDistanceToPoint(point);
      double distanceVel;
      if(angleErr.abs() < 30_deg)
      {
        distanceVel = 200 * m_distancePid->calculateErr(distanceErr.convert(millimeter));
      }


      m_chassis->model->driveVector(distanceVel, angleVel);

      pros::delay(10); // Run the control loop at 10ms intervals
    }

    m_chassis->model->driveVector(0, 0);

  }


  void OdomController::driveToPointAndAngle(Point point)
  {
    driveToPoint(point);
    turnToAngle(point.theta);
  }


}
