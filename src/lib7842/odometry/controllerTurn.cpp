#include "controller.hpp"

namespace lib7842
{

  void OdomController::turnToAngle(QAngle angle, turnFunc_t turnFunc, settleFunc_t settleFunc)
  {
    resetEmergencyAbort();
    angle = rollAngle180(angle);
    turnPid->reset();
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
    turnPid->reset();
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
