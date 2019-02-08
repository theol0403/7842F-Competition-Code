#include "controller.hpp"

namespace lib7842
{

  /**
  * Settle Functions
  */

  settleFunc_t makeSettle(QAngle threshold) {
    return [=](OdomController* that){ return that->m_angleErr.abs() < threshold || that->emergencyAbort(); };
  }

  settleFunc_t makeSettle(QLength threshold) {
    return [=](OdomController* that){ return that->m_distanceErr.abs() < threshold || that->emergencyAbort(); };
  }

  settleFunc_t makeSettle(QLength distanceThreshold, QAngle angleThreshold){
    return [=](OdomController* that){ return (that->m_distanceErr.abs() < distanceThreshold && that->m_angleErr.abs() < angleThreshold) || that->emergencyAbort(); };
  }

  bool turnSettle(OdomController* that) {
    return that->turnPid->isSettled() || that->emergencyAbort();
  }

  bool driveSettle(OdomController* that) {
    return (that->distancePid->isSettled() && that->anglePid->isSettled()) || that->emergencyAbort();
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



  void OdomController::resetVelocity(double vel) { for(int i = 0; i < velFilterSize; i++) { velFilter.filter(vel); } }
  void OdomController::resetVelocityActual() { resetVelocity(getActualVelocity()); }
  void OdomController::resetVelocityMax() { resetVelocity(200); }

  double OdomController::getActualVelocity() {
    return (std::abs(chassis->model->getLeftSideMotor()->getActualVelocity()) + std::abs(chassis->model->getRightSideMotor()->getActualVelocity())) / 2;
  }

  double OdomController::filterVelocity() { return velFilter.filter(getActualVelocity()); }
  double OdomController::getFilteredVelocity() { return velFilter.getOutput(); }

  void OdomController::resetEmergencyAbort() { resetVelocityMax(); }
  bool OdomController::emergencyAbort() { return filterVelocity() < 5; }


  // void OdomController::setFeildColor(autonSides side)
  // {
  //
  // }


}
