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
  	okapiChassis(std::make_shared<ChassisControllerPID>(
		TimeUtilFactory::create(),
		tracker->model,
		std::make_unique<IterativePosPIDController>(*idistancePid), //Distance PID - To mm
		std::make_unique<IterativePosPIDController>(*ianglePid), //Angle PID - To Degree
		std::make_unique<IterativePosPIDController>(*iturnPid), //Turn PID - To Degree
		AbstractMotor::gearset::green,
		ChassisScales{tracker->m_wheelDiam, tracker->m_chassisWidth}))
  {
   okapiChassis->startThread();
  };


  /**
  * Velocity calculations, used for emergency abort
  */
  void OdomController::resetVelocity(double vel) { for(int i = 0; i < velFilterSize; i++) { m_velFilter.filter(vel); } }
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
    okapiChassis->stop();
  }
  bool OdomController::emergencyAbort() {
    if(std::abs(filterVelocity()) < 5) {
      std::cout << "EMERGENCY ABORT" << std::endl;
      return true;
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
