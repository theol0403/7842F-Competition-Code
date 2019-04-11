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
    IterativePosPIDController *iturnPid,
    double islewRate
  ) :
  tracker(itracker),
  distancePid(idistancePid),
  anglePid(ianglePid),
  turnPid(iturnPid),
  slewRate(islewRate),
  pointRadius(1.5_ft)
  {
  };


  void OdomController::resetPid() {
    turnPid->reset();
    distancePid->reset();
    anglePid->reset();
    angleErr = 0_deg;
    distanceErr = 0_in;
    abortTimer.clearHardMark();
  }

  /**
  * returns whether the robot is stuck and the velocity is 0
  * @method OdomController::checkAbort
  * @param  vel - velocity threshold before counting
  * @param  time - time to count before aborting
  * @return - whether to abort or not
  */
  bool OdomController::checkAbort(double vel, QTime time) {
    //if velocity is below a threshold start counting
    if(tracker->getAbsAvgVelocity() <= vel) {
      abortTimer.placeHardMark(); //mark when it first entered 0
    } else {
      abortTimer.clearHardMark(); //reset mark
    }
    //if it has been in velocity for long enough
    if(abortTimer.getDtFromHardMark() > time) {
      std::cerr << "Abort" << std::endl;
      abortTimer.clearHardMark();
      return true;
    }
    return false;
  }

  bool OdomController::checkEmergencyAbort() {
    //only applies when driving and not turning, and not when settling
    if(distanceErr != 0_in) {
      return checkAbort(5, 2_s);
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

    double increment = 0;

    increment = leftOutput - lastVelL;
    if(std::abs(increment) > slewRate) {
      lastVelL += slewRate * sgn(increment);
    } else {
      lastVelL = leftOutput;
    }

    increment = rightOutput - lastVelR;
    if(std::abs(increment) > slewRate) {
      lastVelR += slewRate * sgn(increment);
    } else {
      lastVelR = rightOutput;
    }

    tracker->model->tank(lastVelL, lastVelR);
    // tracker->model->left(leftOutput);
    // tracker->model->right(rightOutput);
  }

  /**
  * execute a list of actions
  * @param actions
  */
  void OdomController::runActions(AsyncActionList actions) {
    for(AsyncActionRef action : actions) { action.get().run(this); }
  }


  /**
  * Relative Position Calcs
  * Does not take side into account
  */
  QAngle OdomController::angleToPoint(QPoint point) {
    QAngle angle = (atan2(point.x.convert(inch) - tracker->state.x.convert(inch), point.y.convert(inch) - tracker->state.y.convert(inch)) * radian) - tracker->state.theta;
    return rollAngle180(angle);
  }

  QLength OdomController::distanceToPoint(QPoint point) {
    return distanceBetweenPoints(tracker->state, point);
  }


  /**
  * Settle Functions
  */
  settleFunc_t OdomController::makeSettle(QAngle threshold) {
    return [=](OdomController* that){ return that->angleErr.abs() < threshold || that->checkEmergencyAbort(); };
  }

  settleFunc_t OdomController::makeSettle(QLength threshold) {
    return [=](OdomController* that){ return that->distanceErr.abs() < threshold || that->checkEmergencyAbort(); };
  }

  settleFunc_t OdomController::makeSettle(QLength distanceThreshold, QAngle angleThreshold){
    return [=](OdomController* that){ return (that->distanceErr.abs() < distanceThreshold && that->angleErr.abs() < angleThreshold) || that->checkEmergencyAbort(); };
  }

  bool OdomController::turnSettle(OdomController* that) {
    return that->turnPid->isSettled() || that->checkEmergencyAbort();
  }

  bool OdomController::driveSettle(OdomController* that) {
    return (that->distancePid->isSettled() /*&& that->anglePid->isSettled()*/) || that->checkEmergencyAbort();
  }



}
