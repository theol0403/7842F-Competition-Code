#include "controller.hpp"

namespace lib7842
{

  void OdomController::turnToAngle(QAngle angle, bool takeShortest, turnFunc_t turnFunc, settleFunc_t settleFunc)
  {
    angle = rollAngle180(angle);
    turnPid->reset();
    do {
      m_angleErr = rollAngle180(angle - chassis->state.theta);
      if(takeShortest && m_angleErr.abs() > 90_deg) {
        m_angleErr += 180_deg;
        m_angleErr = rollAngle180(m_angleErr);
      }
      double turnVel = turnPid->calculateErr(m_angleErr.convert(degree));
      turnFunc(this, turnVel);
      pros::delay(10);
    }
    while(!settleFunc(this));
    turnFunc(this, 0);
  }

  void OdomController::turnAngle(QAngle angle, bool takeShortest, turnFunc_t turnFunc, settleFunc_t settleFunc)
  {
    turnToAngle(angle + chassis->state.theta, takeShortest, turnFunc, settleFunc);
  }


  void OdomController::turnToPoint(qPoint point, bool takeShortest, turnFunc_t turnFunc, settleFunc_t settleFunc)
  {
    turnPid->reset();
    do
    {
      m_angleErr = computeAngleToPoint(point);
      if(takeShortest && m_angleErr.abs() > 90_deg) {
        m_angleErr += 180_deg;
        m_angleErr = rollAngle180(m_angleErr);
      }
      double turnVel = turnPid->calculateErr(m_angleErr.convert(degree));
      turnFunc(this, turnVel);
      pros::delay(10);
    }
    while(!settleFunc(this));
    turnFunc(this, 0);
  }


}
