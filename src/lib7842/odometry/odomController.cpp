#include "odomController.hpp"

namespace lib7842
{

  OdomController::OdomController
  (
    std::shared_ptr<okapi::ChassisControllerPID> chassisController,
    OdomTracker *odomTracker
  ):
  m_chassisController(chassisController),
  m_odomTracker(odomTracker)
  {
  };

  QLength OdomController::computeDistanceToPoint(Point point)
  {
    const QLength xDiff = point.x - m_odomTracker->state.x;
    const QLength yDiff = point.y - m_odomTracker->state.y;
    return std::sqrt(std::pow(xDiff.convert(inch), 2) + std::pow(yDiff.convert(inch), 2)) * inch;
  }

  QAngle OdomController::computeAngleToPoint(Point point)
  {
    return atan2(point.x.convert(inch) - m_odomTracker->state.x.convert(inch), point.y.convert(inch) - m_odomTracker->state.y.convert(inch)) * radian - m_odomTracker->state.theta;
  }


  void OdomController::turnToAngle(QAngle angle)
  {
    m_chassisController->turnAngle(angle - m_odomTracker->state.theta);
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
