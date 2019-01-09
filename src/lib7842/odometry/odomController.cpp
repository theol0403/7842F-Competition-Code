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

  void OdomController::turnAngle(QAngle angle)
  {
    turnToAngle(angle + m_chassis->state.theta);
  }

  void OdomController::turnToAngle(QAngle angle)
  {
    m_turnPid->reset();

    double turnError = 0;
    while(!m_turnPid->isSettled())
    {
      double newOutput = m_turnPid->calculate(angle.convert(degree), m_chassis->state.theta.convert(degree));
      m_chassis->model->rotate(newOutput); //TODO Could be reversed

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


  void OdomController::driveToPoint(Point point)
  {
    turnToPoint(point);

    m_distancePid->setTarget(0);
    m_anglePid->setTarget(0);

    m_distanceUtil.reset();
    m_angleUtil.reset();

    double distanceError = 0;
      double angleError = 0;

    while(!m_distanceUtil.isSettled(distanceError) || !m_angleUtil.isSettled(angleError))
    {
      distanceError = computeDistanceToPoint(point).convert(millimeter);
      angleError = computeAngleToPoint(point).convert(degree);

      if(abs(angleError) > 180)
      {
        distanceError = -distanceError;
      }

      if(distanceError < 500)
      {
        angleError = 0;
      }

      m_chassis->model->driveVector(m_distancePid->step(-distanceError), m_anglePid->step(-angleError));

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
