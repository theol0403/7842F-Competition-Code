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
      that->tracker->model->left(leftVel);
      that->tracker->model->right(rightVel);
    } else if(leftRatio < rightRatio) {
      that->tracker->model->left(-leftVel);
      that->tracker->model->right(-rightVel);
    } else {
      that->tracker->model->rotate(turnVel);
    }
  };}

  void pointTurn(OdomController* that, double turnVel) {
    that->tracker->model->rotate(turnVel);
  }

  void leftPivot(OdomController* that, double turnVel) {
    that->tracker->model->left(turnVel * 2);
  }

  void rightPivot(OdomController* that, double turnVel) {
    that->tracker->model->right(-turnVel * 2);
  }

  turnCalc_t angleCalc(QAngle angle) {
    angle = rollAngle180(angle);
    return [=](OdomController* that) {
      return rollAngle180(that->setSide(angle) - that->tracker->state.theta); };
  }

  turnCalc_t angleCalc(qPoint point) {
    return [=](OdomController* that) { return that->computeAngleToPoint(that->setSide(point)); };
  }

  void OdomController::turn(turnCalc_t turnCalc, turnFunc_t turnFunc, settleFunc_t settleFunc, AsyncActionList actions)
  {
    resetEmergencyAbort();
    do {
      m_angleErr = turnCalc(this);
      double turnVel = turnPid->calculateErr(m_angleErr.convert(degree));
      turnFunc(this, turnVel);
      runActions(actions);
      pros::delay(10);
    }
    while(!settleFunc(this));
    turnFunc(this, 0);
  }

  void OdomController::turnToAngle(QAngle angle, turnFunc_t turnFunc, settleFunc_t settleFunc, AsyncActionList actions)
  {
    turn(angleCalc(angle), turnFunc, settleFunc, actions);
  }

  void OdomController::turnAngle(QAngle angle, turnFunc_t turnFunc, settleFunc_t settleFunc, AsyncActionList actions)
  {
    turn(angleCalc(angle + setSide(tracker->state.theta)), turnFunc, settleFunc, actions);
  }

  void OdomController::turnToPoint(qPoint point, turnFunc_t turnFunc, settleFunc_t settleFunc, AsyncActionList actions)
  {
    turn(angleCalc(point), turnFunc, settleFunc, actions);
  }



}
