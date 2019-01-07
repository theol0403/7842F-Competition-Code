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

  double OdomController::computeAngleToPoint(Point point)
  {
    return atan2(point.x - m_odomTracker->state.x.convert(inch), point.y - m_odomTracker->state.y.convert(inch)) - m_odomTracker->state.t.convert(inch)heta;
  }


  void OdomController::turnToAngle(QAngle angle)
  {
    m_chassisController->turnAngle(angle - (m_odomTracker->state.theta * radian));
  }

  void OdomController::turnToPoint(Point point)
  {
    m_chassisController->turnAngle(computeAngleToPoint(point) * radian);
  }


  void OdomController::driveToPoint(Point point)
  {
    double wantedDistance = computeDistanceToPoint(point);
    double wantedAngle = computeAngleToPoint(point);

    m_chassisController->turnAngle(wantedAngle * radian);

    m_chassisController->moveDistance(wantedDistance * inch);
  }


  void OdomController::driveToPointAndAngle(Point point)
  {
    m_chassisController->turnAngle(computeAngleToPoint(point) * radian);
    m_chassisController->moveDistance(computeDistanceToPoint(point) * inch);
    turnToAngle(point.theta * radian);
  }


}
