#include "controller.hpp"

namespace lib7842
{

  void OdomController::turnToAngle(QAngle angle, turnFunc_t turnFunc, settleFunc_t settleFunction)
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

  void OdomController::turnAngle(QAngle angle, turnFunc_t turnFunc, settleFunc_t settleFunction)
  {
    turnToAngle(angle + chassis->state.theta, settleFunction);
  }


  void OdomController::turnToPoint(qPoint point, turnFunc_t turnFunc, settleFunc_t settleFunction)
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
