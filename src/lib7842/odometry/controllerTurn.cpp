#include "controller.hpp"

namespace lib7842
{

  void OdomController::turnPoint(double turnVel) {
    chassis->model->rotate(turnVel);
  }

  void OdomController::turnPivotLeft(double turnVel) {
    chassis->model->left(turnVel * 2);
  }

  void OdomController::turnPivotRight(double turnVel) {
    chassis->model->right(-turnVel * 2);
  }

  void OdomController::turnToAngle(QAngle angle, settleFunc_t settleFunction)
  {
    angle = rollAngle180(angle);
    turnPid->reset();
    do {
      m_angleErr = rollAngle180(angle - chassis->state.theta);
      double turnVel = turnPid->calculateErr(m_angleErr.convert(degree));
      chassis->model->rotate(turnVel);
      pros::delay(10);
    }
    while(!settleFunction(this));
    chassis->model->rotate(0);
  }

  void OdomController::turnAngle(QAngle angle, settleFunc_t settleFunction)
  {
    turnToAngle(angle + chassis->state.theta, settleFunction);
  }


  void OdomController::turnToPoint(qPoint point, settleFunc_t settleFunction)
  {
    turnPid->reset();
    do
    {
      m_angleErr = computeAngleToPoint(point);
      double turnVel = turnPid->calculateErr(m_angleErr.convert(degree));
      chassis->model->rotate(turnVel);
      pros::delay(10);
    }
    while(!settleFunction(this));
    chassis->model->rotate(0);
  }


}
