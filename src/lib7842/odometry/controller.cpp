#include "controller.hpp"

namespace lib7842
{

  OdomController::OdomController(
    OdomTracker *ichassis,
    PID *idistancePid,
    PID *ianglePid,
    PID *iturnPid
  ) :
  chassis(ichassis),
  distancePid(idistancePid),
  anglePid(ianglePid),
  turnPid(iturnPid)
  {};


  bool OdomController::turnSettle(OdomController* that) {
    return that->turnPid->isSettled();
  }

  bool OdomController::turnNoSettle(OdomController* that) {
    return abs(that->turnPid->getError()) < 10;
  }

  bool OdomController::driveSettle(OdomController* that) {
    return that->distancePid->isSettled() && that->anglePid->isSettled();
  }

  bool OdomController::driveNoSettle(OdomController* that) {
    return abs(that->distancePid->getError()) < 10; //mm
  }

  std::function<bool(OdomController*)> OdomController::createTurnSettle(QAngle threshold)
  {
    return [=](OdomController* that){ return abs(that->turnPid->getError()) < threshold.convert(degree); };
  }

  std::function<bool(OdomController*)> OdomController::createDriveSettle(QLength threshold)
  {
    return [=](OdomController* that){ return abs(that->distancePid->getError()) < threshold.convert(millimeter); };
  }


  QAngle OdomController::computeAngleToPoint(qPoint point)
  {
    QAngle angle = (atan2(point.x.convert(inch) - chassis->state.x.convert(inch), point.y.convert(inch) - chassis->state.y.convert(inch)) * radian) - chassis->state.theta;
    return rollAngle180(angle);
  }

  QLength OdomController::computeDistanceToPoint(qPoint point)
  {
    return computeDistanceBetweenPoints(chassis->state, point);
  }


  void OdomController::normalizeDrive(double &distanceVel, double &angleVel)
  {
    double maxMag = std::max(fabs(angleVel), fabs(distanceVel));
    if(maxMag > chassis->model->maxVelocity) {
      distanceVel = (distanceVel / maxMag) * chassis->model->maxVelocity;
      angleVel = (angleVel / maxMag) * chassis->model->maxVelocity;
    }
  }


}
