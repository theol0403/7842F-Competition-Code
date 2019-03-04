#include "controller.hpp"

namespace lib7842
{

  /**
  * Odom Controller
  */
  OdomController::OdomController(
    OdomTracker *itracker,
    IterativePosPIDController *idistancePid,
    IterativePosPIDController *ianglePid,
    IterativePosPIDController *iturnPid
  ) :
  tracker(itracker),
  distancePid(idistancePid),
  anglePid(ianglePid),
  turnPid(iturnPid),
  m_velFilter(30)
  {
  };


  /**
  * Velocity calculations, used for emergency abort
  */
  void OdomController::resetVelocity(double vel) { m_velFilter.resetTo(vel); }
  void OdomController::resetVelocityActual() { resetVelocity(getActualVelocity()); }
  void OdomController::resetVelocityMax() { resetVelocity(200); }

  double OdomController::getActualVelocity() {
    return (std::abs(tracker->model->getLeftSideMotor()->getActualVelocity()) + std::abs(tracker->model->getRightSideMotor()->getActualVelocity())) / 2;
  }

  double OdomController::filterVelocity() { return m_velFilter.filter(getActualVelocity()); }
  double OdomController::getFilteredVelocity() { return m_velFilter.getOutput(); }

  void OdomController::resetEmergencyAbort() {
    resetVelocityMax();
    turnPid->reset();
    distancePid->reset();
    anglePid->reset();
  }

  bool OdomController::emergencyAbort() {
    if(std::abs(filterVelocity()) <= 0) {
      std::cout << "EMERGENCY ABORT - n/a" << std::endl;
      //return true;
      return false;
    }
    return false;
  }

  /**
  * custom vector control which allows yaw to have priority over forwardSped
  * @param forwardSpeed
  * @param yaw
  */
  void OdomController::driveVector(double forwardSpeed, double yaw)
  {
    double leftOutput = forwardSpeed + yaw;
    double rightOutput = forwardSpeed - yaw;
    double maxInputMag = std::max<double>(std::abs(leftOutput), std::abs(rightOutput));
    if (maxInputMag > 1) {
      leftOutput /= maxInputMag;
      rightOutput /= maxInputMag;
    }
    tracker->model->left(leftOutput);
    tracker->model->right(rightOutput);
  }

  /**
  * execute a list of actions
  * @param actions
  */
  void OdomController::runActions(AsyncActionList actions)
  {
    for(AsyncActionRef action : actions) { action.get().run(this); }
  }


  /**
  * Relative Position Calcs
  * Does not take side into account
  */
  QAngle OdomController::computeAngleToPoint(qPoint point) {
    QAngle angle = (atan2(point.x.convert(inch) - tracker->state.x.convert(inch), point.y.convert(inch) - tracker->state.y.convert(inch)) * radian) - tracker->state.theta;
    return rollAngle180(angle);
  }

  QLength OdomController::computeDistanceToPoint(qPoint point) {
    return computeDistanceBetweenPoints(tracker->state, point);
  }


  /**
  * Settle Functions
  */
  settleFunc_t OdomController::makeSettle(QAngle threshold) {
    return [=](OdomController* that){ return that->m_angleErr.abs() < threshold || that->emergencyAbort(); };
  }

  settleFunc_t OdomController::makeSettle(QLength threshold) {
    return [=](OdomController* that){ return that->m_distanceErr.abs() < threshold || that->emergencyAbort(); };
  }

  settleFunc_t OdomController::makeSettle(QLength distanceThreshold, QAngle angleThreshold){
    return [=](OdomController* that){ return (that->m_distanceErr.abs() < distanceThreshold && that->m_angleErr.abs() < angleThreshold) || that->emergencyAbort(); };
  }

  bool OdomController::turnSettle(OdomController* that) {
    return that->turnPid->isSettled() || that->emergencyAbort();
  }

  bool OdomController::driveSettle(OdomController* that) {
    return (that->distancePid->isSettled() && that->anglePid->isSettled()) || that->emergencyAbort();
  }



}
