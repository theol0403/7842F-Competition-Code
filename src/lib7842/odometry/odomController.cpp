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
    QAngle wantedAngle = (atan2(point.x.convert(inch) - m_odomTracker->state.x.convert(inch), point.y.convert(inch) - m_odomTracker->state.y.convert(inch)) * radian) - m_odomTracker->state.theta;
    if(wantedAngle.abs() > 180_deg)
    {
      wantedAngle -= 360_deg * sgn(wantedAngle.convert(degree));
    }
    return wantedAngle;
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
    int direction = 1;
    QAngle wantedAngle = computeAngleToPoint(point);
    if(wantedAngle.abs() > 90_deg)
    {
      wantedAngle -= 180_deg * sgn(wantedAngle.convert(degree));
      direction = -1;
    }
    m_chassisController->turnAngle(wantedAngle);

    m_chassisController->moveDistance(computeDistanceToPoint(point) * direction);
  }


  void OdomController::driveToPointAndAngle(Point point)
  {
    driveToPoint(point);
    turnToAngle(point.theta);
  }


}
