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

  void OdomController::resetEmergencyAbort() { resetVelocityMax(); }
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
  * sets the side
  * @param side
  */
  void OdomController::setSide(autonSides side)
  {
    m_autonSide = side;
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
  * Wrapper Functions which mirror side
  */
  void OdomController::turn(angleCalc_t turnCalc, turnFunc_t turnFunc, settleFunc_t settleFunc, AsyncActionList actions) {
    m_turn(turnCalc, turnFunc, settleFunc, actions);
  }
  void OdomController::turnToAngle(QAngle angle, turnFunc_t turnFunc, settleFunc_t settleFunc, AsyncActionList actions) {
    m_turnToAngle(mirrorSide(angle, m_autonSide), turnFunc, settleFunc, actions);
  }
  void OdomController::turnAngle(QAngle angle, turnFunc_t turnFunc, settleFunc_t settleFunc, AsyncActionList actions) {
    m_turnAngle(mirrorSide(angle, m_autonSide), turnFunc, settleFunc, actions);
  }
  void OdomController::turnToPoint(qPoint point, turnFunc_t turnFunc, settleFunc_t settleFunc, AsyncActionList actions) {
    m_turnToPoint(mirrorSide(point, m_autonSide), turnFunc, settleFunc, actions);
  }

  void OdomController::driveDistanceAtAngle(QLength distance, angleCalc_t turnCalc, double turnScale, settleFunc_t settleFunc, AsyncActionList actions) {
    m_driveDistanceAtAngle(distance, turnCalc, turnScale, settleFunc, actions);
  }
  void OdomController::driveDistance(QLength distance, settleFunc_t settleFunc, AsyncActionList actions) {
    m_driveDistance(distance, settleFunc, actions);
  }
  void OdomController::driveForTime(int time, double vel, AsyncActionList actions) {
    m_driveForTime(time, vel, actions);
  }
  void OdomController::driveForTimeAtAngle(int time, double vel, angleCalc_t turnCalc, double turnScale, AsyncActionList actions) {
    m_driveForTimeAtAngle(time, vel, turnCalc, turnScale, actions);
  }
  void OdomController::allignToAngle(QAngle angle, double vel, double velThresh) {
    m_allignToAngle(mirrorSide(angle, m_autonSide), vel, velThresh);
  }

  void OdomController::driveToPoint(qPoint targetPoint, double turnScale, settleFunc_t settleFunc, AsyncActionList actions) {
    m_driveToPoint(mirrorSide(targetPoint, m_autonSide), turnScale, settleFunc, actions);
  }
  void OdomController::driveToPointSimple(qPoint targetPoint, double turnScale, settleFunc_t settleFunc, AsyncActionList actions) {
    m_driveToPointSimple(mirrorSide(targetPoint, m_autonSide), turnScale, settleFunc, actions);
  }

  void OdomController::drivePath(Path path, double turnScale, settleFunc_t moveOnSettle, settleFunc_t finalSettle, AsyncActionList actions) {
    m_drivePath(mirrorSide(path, m_autonSide), turnScale, moveOnSettle, finalSettle, actions);
  }
  void OdomController::drivePathSimple(Path path, double turnScale, settleFunc_t moveOnSettle, settleFunc_t finalSettle, AsyncActionList actions) {
    m_drivePathSimple(mirrorSide(path, m_autonSide), turnScale, moveOnSettle, finalSettle, actions);
  }





}
