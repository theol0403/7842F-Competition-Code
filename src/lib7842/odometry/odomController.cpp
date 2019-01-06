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

  double OdomController::computeDistanceToPoint(const OdomPoint point)
  {
    const double xDiff = point.x - m_odomTracker->trackingPoint.x;
    const double yDiff = point.y - m_odomTracker->trackingPoint.y;
    return std::sqrt((xDiff * xDiff) + (yDiff * yDiff));
  }

  double OdomController::computeAngleToPoint(const OdomPoint point)
  {
    const double xDiff = point.x - m_odomTracker->trackingPoint.x;
    const double yDiff = point.y - m_odomTracker->trackingPoint.y;
    return std::atan2(yDiff, xDiff) - m_odomTracker->trackingPoint.theta;
  }


  void OdomController::driveToPoint(const QLength ix,
                                            const QLength iy,
                                            const bool ibackwards,
                                            const QLength ioffset) {
  auto [length, angle] = OdomMath::computeDistanceAndAngleToPoint(ix, iy, odom->getState());

  if (ibackwards) {
    length *= -1;
    angle += 180_deg;
  }

  logger->info("OdomChassisControllerPID: Computed length of " +
               std::to_string(length.convert(meter)) + " meters and angle of " +
               std::to_string(angle.convert(degree)) + " degrees");

  if (angle.abs() > turnThreshold) {
    ChassisControllerPID::turnAngle(angle);
  }

  if ((length - ioffset).abs() > moveThreshold) {
    ChassisControllerPID::moveDistance(length - ioffset);
  }
}



}
