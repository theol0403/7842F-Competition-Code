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

  double OdomController::computeDistanceToPoint(OdomPoint point)
  {
    const double xDiff = point.x - m_odomTracker->trackingPoint.x;
    const double yDiff = point.y - m_odomTracker->trackingPoint.y;
    return std::sqrt((xDiff * xDiff) + (yDiff * yDiff));
  }

  double OdomController::computeAngleToPoint(OdomPoint point)
  {
    return atan2(point.x - m_odomTracker->trackingPoint.x, point.y - m_odomTracker->trackingPoint.y) - m_odomTracker->trackingPoint.theta;
  }


  void OdomController::turnToAngle(QAngle angle)
  {
    m_chassisController->turnAngle(angle - (m_odomTracker->trackingPoint.theta * radian));
  }

  void OdomController::turnToPoint(OdomPoint point)
  {
    m_chassisController->turnAngle(computeAngleToPoint(point) * radian);
  }


  void OdomController::driveToPoint(OdomPoint point)
  {
    double wantedDistance = computeDistanceToPoint(point);
    double wantedAngle = computeAngleToPoint(point);

    m_chassisController->turnAngle(wantedAngle * radian);

    m_chassisController->moveDistance(wantedDistance * inch);
  }


  void OdomController::driveToPointAndAngle(OdomPoint point)
  {
    m_chassisController->turnAngle(computeAngleToPoint(point) * radian);
    m_chassisController->moveDistance(computeDistanceToPoint(point) * inch);
    turnToAngle(point.theta * radian);
  }


}
