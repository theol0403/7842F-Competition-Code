#include "odomController.hpp"

namespace lib7842
{

  OdomController::OdomController(
    OdomTracker *chassis,
    std::unique_ptr<IterativePosPIDController> idistanceController,
    std::unique_ptr<IterativePosPIDController> iangleController,
    std::unique_ptr<IterativePosPIDController> iturnController

  )
  :
  m_chassis(chassis),
  m_distancePid(std::move(idistanceController)),
  m_anglePid(std::move(iangleController)),
  m_turnPid(std::move(iturnController)),
  m_distanceUtil(std::make_unique<Timer>(), 1, 1, 250_ms),
  m_angleUtil(std::make_unique<Timer>(), 1, 1, 250_ms),
  m_turnUtil(std::make_unique<Timer>(), 1, 1, 250_ms)
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
    m_turnPid->setTarget(0);
    m_turnUtil.reset();
    double turnError = 0;
    while(!m_turnUtil.isSettled(turnError))
    {
      turnError = m_chassis->state.theta.convert(degree) - angle.convert(degree);
      double newOutput = m_turnPid->step(turnError);
      m_chassis->model->rotate(newOutput);

      pros::delay(10); // Run the control loop at 10ms intervals
    }
  }

  void OdomController::turnToPoint(Point point)
  {
    turnToAngle(computeAngleToPoint(point) + m_chassis->state.theta);
  }


  QLength OdomController::computeDistanceToPoint(Point point)
  {
    const QLength xDiff = point.x - m_chassis->state.x;
    const QLength yDiff = point.y - m_chassis->state.y;
    return std::sqrt(std::pow(xDiff.convert(inch), 2) + std::pow(yDiff.convert(inch), 2)) * inch;
  }

  QLength OdomController::computeDistanceBetweenPoints(Point firstPoint, Point secondPoint)
  {
    const QLength xDiff = secondPoint.x - firstPoint.x;
    const QLength yDiff = secondPoint.y - firstPoint.y;
    return std::sqrt(std::pow(xDiff.convert(inch), 2) + std::pow(yDiff.convert(inch), 2)) * inch;
  }


  void OdomController::driveToPoint(Point point)
  {
    m_distancePid->setTarget(0);
    m_anglePid->setTarget(0);
    m_distanceUtil.reset();
    m_angleUtil.reset();
    double distanceError = 0;
    double angleError = 0;
    
    while(!m_distanceUtil.isSettled(distanceError) || !m_angleUtil.isSettled(angleError))
    {
      double forwardSpeed = m_distancePid->step(-computeDistanceToPoint(point).convert(inch));
      double angleSpeed = m_anglePid->step(-computeAngleToPoint(point).convert(degree));
      m_chassis->model->driveVector(forwardSpeed, angleSpeed);

      pros::delay(10); // Run the control loop at 10ms intervals
    }

  }


  void OdomController::driveToPointAndAngle(Point point)
  {
    driveToPoint(point);
    turnToAngle(point.theta);
  }


}
