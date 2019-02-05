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

  void OdomController::driveVector(double forwardSpeed, double yaw)
  {
    double leftOutput = forwardSpeed + yaw;
    double rightOutput = forwardSpeed - yaw;
    // double maxInputMag = std::max<double>(std::abs(leftOutput), std::abs(rightOutput));
    // if (maxInputMag > 1) {
    //   leftOutput /= maxInputMag;
    //   rightOutput /= maxInputMag;
    // }

    chassis->model->left(leftOutput);
    chassis->model->right(rightOutput);
  }

}
