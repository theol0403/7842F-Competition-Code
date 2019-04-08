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
  m_pointRadius(1.5_ft)
  {
  };


  /**
  * Velocity calculations and checkForAbort
  */
  double OdomController::getLeftVelocity() { return tracker->model->getLeftSideMotor()->getActualVelocity();}
  double OdomController::getRightVelocity() { return tracker->model->getRightSideMotor()->getActualVelocity();}
  double OdomController::getAvgVelocity() { return (getLeftVelocity() + getRightVelocity()) / 2;}
  double OdomController::getAbsLeftVelocity() { return std::abs(tracker->model->getLeftSideMotor()->getActualVelocity());}
  double OdomController::getAbsRightVelocity() { return std::abs(tracker->model->getRightSideMotor()->getActualVelocity());}
  double OdomController::getAbsAvgVelocity() { return (getAbsLeftVelocity() + getAbsRightVelocity()) / 2;}

  void OdomController::resetPid() {
    turnPid->reset();
    distancePid->reset();
    anglePid->reset();
    m_angleErr = 0_deg;
    m_distanceErr = 0_in;
  }

  bool OdomController::checkForAbort() {
    //if distance error is not 0 such as in a turn
    //and if the distance error is inside the radius
    if(m_distanceErr != 0_in && m_distanceErr < m_pointRadius) {
      //if velocity is below a threshold start counting
      if(getAbsAvgVelocity() <= 0) {
        abortTimer.placeHardMark(); //mark when it first entered 0
      } else {
        abortTimer.clearHardMark(); //reset mark
      }
      //if it has been in 0 velocity for long enough
      if(abortTimer.getDtFromHardMark() > 3_s) {
        return true;
        std::cout << "Emergency Abort" << std::endl;
      }
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
    return [=](OdomController* that){ return that->m_angleErr.abs() < threshold || that->checkForAbort(); };
  }

  settleFunc_t OdomController::makeSettle(QLength threshold) {
    return [=](OdomController* that){ return that->m_distanceErr.abs() < threshold || that->checkForAbort(); };
  }

  settleFunc_t OdomController::makeSettle(QLength distanceThreshold, QAngle angleThreshold){
    return [=](OdomController* that){ return (that->m_distanceErr.abs() < distanceThreshold && that->m_angleErr.abs() < angleThreshold) || that->checkForAbort(); };
  }

  bool OdomController::turnSettle(OdomController* that) {
    return that->turnPid->isSettled() || that->checkForAbort();
  }

  bool OdomController::driveSettle(OdomController* that) {
    return (that->distancePid->isSettled() && that->anglePid->isSettled()) || that->checkForAbort();
  }



}
