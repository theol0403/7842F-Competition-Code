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
  m_turnPid(std::move(iturnController))
  {
  };

  QAngle OdomController::computeAngleOfPoint(Point point)
  {
    return atan2(point.x.convert(inch) - m_chassis->state.x.convert(inch), point.y.convert(inch) - m_chassis->state.y.convert(inch)) * radian;
  }

  void OdomController::turnAngle(QAngle angle)
  {
    turnToAngle(angle + m_chassis->state.theta);
  }

  void OdomController::turnToAngle(QAngle angle)
  {
    m_turnPid->setTarget(angle.convert(degree));

    while(!m_turnPid->isSettled())
    {
      double newOutput = m_turnPid->step(m_chassis->state.theta.convert(degree));
      m_chassis->model->rotate(newOutput);

      pros::delay(10); // Run the control loop at 10ms intervals
    }
  }

  void OdomController::turnToPoint(Point point)
  {
    turnToAngle(computeAngleOfPoint(point));
  }


  QLength OdomController::computeDistanceToPoint(Point point)
  {
    const QLength xDiff = point.x - m_chassis->state.x;
    const QLength yDiff = point.y - m_chassis->state.y;
    return std::sqrt(std::pow(xDiff.convert(inch), 2) + std::pow(yDiff.convert(inch), 2)) * inch;
  }

  QLength OdomController::computeDistanceOfPoint(Point point)
  {
    return std::sqrt(std::pow(point.x.convert(inch), 2) + std::pow(point.y.convert(inch), 2)) * inch;
  }



  void OdomController::driveToPoint(Point point)
  {

    m_distancePid->setTarget(computeDistanceOfPoint(point).convert(inch));
    m_anglePid->setTarget(computeAngleOfPoint(point).convert(degree));

    while(!m_distancePid->isSettled() || !m_anglePid->isSettled())
    {
      double newDistance = computeDistanceOfPoint(m_chassis->state).convert(inch);
      double newAngle = m_chassis->state.theta.convert(degree);
      m_chassis->model->driveVector(m_distancePid->step(newDistance), m_anglePid->step(newAngle));

      pros::delay(10); // Run the control loop at 10ms intervals
    }
  }


  void OdomController::driveToPointAndAngle(Point point)
  {
    driveToPoint(point);
    turnToAngle(point.theta);
  }


}
