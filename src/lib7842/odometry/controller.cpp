#include "controller.hpp"

namespace lib7842
{

  settleFunc_t makeSettle(QAngle threshold) {
    return [=](OdomController* that){ return that->m_angleErr.abs() < threshold; };
  }

  settleFunc_t makeSettle(QLength threshold) {
    return [=](OdomController* that){ return that->m_distanceErr.abs() < threshold; };
  }

  settleFunc_t makeSettle(QLength distanceThreshold, QAngle angleThreshold){
    return [=](OdomController* that){ return that->m_distanceErr.abs() < distanceThreshold && that->m_angleErr.abs() < angleThreshold; };
  }


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

  bool OdomController::driveSettle(OdomController* that) {
    return that->distancePid->isSettled() && that->anglePid->isSettled();
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


  void OdomController::normalizeDrive(double &distanceVel, double &angleVel) {
    double maxMag = std::max(fabs(angleVel), fabs(distanceVel));
    if(maxMag > 1.0) {
      distanceVel = (distanceVel / maxMag);
      angleVel = (angleVel / maxMag);
    }
  }


}
