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

  /**
  * Trigger Functions
  */
  triggerFunc_t makeTrigger(qPoint point, QLength distanceThresh) {
    return [=](OdomController* that){ return that->computeDistanceToPoint(point) < distanceThresh; };
  }

  triggerFunc_t makeTrigger(qPoint point, QLength distanceThresh, QAngle angleThresh) {
    return [=](OdomController* that){ return that->computeDistanceToPoint(point) < distanceThresh && that->computeAngleToPoint(point).abs() < angleThresh; };
  }

  triggerFunc_t makeTrigger(qPoint point, QAngle angleThresh) {
    return [=](OdomController* that){ return that->computeAngleToPoint(point).abs() < angleThresh; };
  }

  triggerFunc_t makeTrigger(QAngle angle, QAngle angleThresh) {
    return [=](OdomController* that){ return that->tracker->getTheta() > angle - angleThresh && that->tracker->getTheta() < angle + angleThresh; };
  }

  /**
  * Odom Controller
  */
  OdomController::OdomController(
    OdomTracker *itracker,
    PID *idistancePid,
    PID *ianglePid,
    PID *iturnPid
  ) :
  tracker(itracker),
  distancePid(idistancePid),
  anglePid(ianglePid),
  turnPid(iturnPid)
  {};

  /**
  * Relative Position Calcs
  */
  QAngle OdomController::computeAngleToPoint(qPoint point) {
    QAngle angle = (atan2(point.x.convert(inch) - tracker->state.x.convert(inch), point.y.convert(inch) - tracker->state.y.convert(inch)) * radian) - tracker->state.theta;
    return rollAngle180(angle);
  }

  QLength OdomController::computeDistanceToPoint(qPoint point) {
    return computeDistanceBetweenPoints(tracker->state, point);
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
  * Velocity calculations, used for emergency abort
  */
  void OdomController::resetVelocity(double vel) { for(int i = 0; i < velFilterSize; i++) { velFilter.filter(vel); } }
  void OdomController::resetVelocityActual() { resetVelocity(getActualVelocity()); }
  void OdomController::resetVelocityMax() { resetVelocity(200); }

  double OdomController::getActualVelocity() {
    return (std::abs(tracker->model->getLeftSideMotor()->getActualVelocity()) + std::abs(tracker->model->getRightSideMotor()->getActualVelocity())) / 2;
  }

  double OdomController::filterVelocity() { return velFilter.filter(getActualVelocity()); }
  double OdomController::getFilteredVelocity() { return velFilter.getOutput(); }

  void OdomController::resetEmergencyAbort() { resetVelocityMax(); }
  bool OdomController::emergencyAbort() {
    if(std::abs(filterVelocity()) < 5)
    {
      std::cout << "EMERGENCY ABORT" << std::endl;
      return true;
    }
    return false;
  }


  // void OdomController::checkTriggers(std::vector<triggerAction_t> &triggerActions)
  // {
  //   for(triggerAction_t &triggerAction : triggerActions)
  //   {
  //     if(triggerAction.trigger(this) && !triggerAction.triggered)
  //     {
  //       triggerAction.action(this);
  //       triggerAction.triggered = true;
  //     }
  //   }
  // }


  // void OdomController::setFeildColor(autonSides side)
  // {
  //
  // }


}
