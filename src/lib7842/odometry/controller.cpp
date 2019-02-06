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

  bool turnSettle(OdomController* that) {
    return that->turnPid->isSettled();
  }

  bool driveSettle(OdomController* that) {
    return that->distancePid->isSettled() && that->anglePid->isSettled();
  }


  turnFunc_t makeArc(double leftRatio, double rightRatio) { return [=](OdomController* that, double turnVel) {
    if(leftRatio > rightRatio) {
      that->chassis->model->left(turnVel * leftRatio);
      that->chassis->model->right(turnVel * rightRatio);
    } else if(leftRatio < rightRatio) {
      that->chassis->model->left(-turnVel * leftRatio);
      that->chassis->model->right(-turnVel * rightRatio);
    } else {
      that->chassis->model->rotate(turnVel);
    }
  };}

  void pointTurn(OdomController* that, double turnVel) {
    that->chassis->model->rotate(turnVel);
  }

  void leftPivot(OdomController* that, double turnVel) {
    that->chassis->model->left(turnVel * 2);
  }

  void rightPivot(OdomController* that, double turnVel) {
    that->chassis->model->right(-turnVel * 2);
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

}
