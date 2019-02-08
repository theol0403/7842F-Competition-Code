#include "controller.hpp"

namespace lib7842
{

  turnFunc_t makeArc(double leftRatio, double rightRatio) { return [=](OdomController* that, double turnVel) {
    turnVel = std::clamp(turnVel, -1.0, 1.0);
    double leftVel = turnVel * leftRatio;
    double rightVel = turnVel * rightRatio;
    double maxMag = std::max(std::abs(leftVel), std::abs(rightVel));
    if(maxMag > 1) {
      leftVel = (leftVel / maxMag);
      rightVel = (rightVel / maxMag);
    }
    if(leftRatio > rightRatio) {
      that->chassis->model->left(leftVel);
      that->chassis->model->right(rightVel);
    } else if(leftRatio < rightRatio) {
      that->chassis->model->left(-leftVel);
      that->chassis->model->right(-rightVel);
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



  void OdomController::turnToAngle(QAngle angle, turnFunc_t turnFunc, settleFunc_t settleFunc)
  {
    resetEmergencyAbort();
    angle = rollAngle180(angle);
    do {
      m_angleErr = rollAngle180(angle - chassis->state.theta);
      double turnVel = turnPid->calculateErr(m_angleErr.convert(degree));
      turnFunc(this, turnVel);
      pros::delay(10);
    }
    while(!settleFunc(this));
    turnFunc(this, 0);
  }

  void OdomController::turnAngle(QAngle angle, turnFunc_t turnFunc, settleFunc_t settleFunc)
  {
    turnToAngle(angle + chassis->state.theta, turnFunc, settleFunc);
  }



  void OdomController::turnToPoint(qPoint point, turnFunc_t turnFunc, settleFunc_t settleFunc)
  {
    resetEmergencyAbort();
    do
    {
      m_angleErr = computeAngleToPoint(point);
      double turnVel = turnPid->calculateErr(m_angleErr.convert(degree));
      turnFunc(this, turnVel);
      pros::delay(10);
    }
    while(!settleFunc(this));
    turnFunc(this, 0);
  }




}
