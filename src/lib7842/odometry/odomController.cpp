#include "odomController.hpp"

namespace lib7842
{

  OdomController::OdomController(
    OdomTracker *odom,
    std::unique_ptr<IterativePosPIDController> idistanceController,
    std::unique_ptr<IterativePosPIDController> iturnController,
    std::unique_ptr<IterativePosPIDController> iangleController
  )
  :
  m_odom(odom),
  m_distancePid(std::move(idistanceController)),
  m_turnPid(std::move(iturnController)),
  m_anglePid(std::move(iangleController))
  {
  };


  void OdomController::turnToAngle(QAngle angle)
  {
    m_anglePid->setTarget(angle.convert(degree));

    while (!m_anglePid->isSettled())
    {
      double newOutput = m_anglePid->step(m_odom->state.theta.convert(degree));
      m_odom->chassis->rotate(newOutput);

      pros::delay(10); // Run the control loop at 10ms intervals
    }
  }

  QLength OdomController::computeDistanceToPoint(Point point)
  {
    const QLength xDiff = point.x - m_odom->state.x;
    const QLength yDiff = point.y - m_odom->state.y;
    return std::sqrt(std::pow(xDiff.convert(inch), 2) + std::pow(yDiff.convert(inch), 2)) * inch;
  }

  QAngle OdomController::computeAngleToPoint(Point point)
  {
    return atan2(point.x.convert(inch) - m_odom->state.x.convert(inch), point.y.convert(inch) - m_odom->state.y.convert(inch)) * radian - m_odom->state.theta;
  }


  void OdomController::turnToAngle(QAngle angle)
  {
    m_chassisController->turnAngle(angle - m_odom->state.theta);
  }

  void OdomController::turnToPoint(Point point)
  {
    m_chassisController->turnAngle(computeAngleToPoint(point));
  }


  void OdomController::driveToPoint(Point point)
  {
    m_chassisController->turnAngle(computeAngleToPoint(point));

    m_chassisController->moveDistance(computeDistanceToPoint(point));
  }


  void OdomController::driveToPointAndAngle(Point point)
  {
    driveToPoint(point);
    turnToAngle(point.theta);
  }


}
