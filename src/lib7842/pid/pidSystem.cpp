#include "pidSystem.hpp"

namespace lib7842
{

  PID::PID(double Kp, double Kd, double settledError, double settledD, QTime settledTime)
  : m_timer(),
  m_Kp(Kp),
  m_Kd(Kd),
  m_settledUtil(std::make_unique<okapi::Timer>(), settledError, settledD, settledTime)
  {
    m_lastTime = m_timer.elapsed();
  }


  double PID::calculateErr(double ierror)
  {
    m_Error = ierror;
    double deltaTime = m_timer.elapsed() - m_lastTime;
    m_lastTime = m_timer.elapsed();

    m_derivative = (m_Error - m_lastError) / deltaTime;
    m_lastError = m_Error;

    double finalPower = (m_Error * m_Kp) + (m_derivative * m_Kd);
    if(fabs(finalPower) > 127)
    {
      finalPower = sgn(finalPower) * 127;
    }

    m_isSettled = m_settledUtil.isSettled(m_Error);

    return finalPower;
  }


  double PID::calculate(double wantedPos, double currentPos)
  {
    return calculateErr(wantedPos - currentPos);
  }


  double PID::getError()
  {
    return m_Error;
  }


  void PID::reset()
  {
    m_Error = 0;
    m_lastError = 0;
    m_lastTime = m_timer.elapsed();
    m_derivative = 0;
    m_settledUtil.reset();
    m_isSettled = false;
  }

  bool PID::isSettled()
  {
    return m_isSettled;
  }


}
