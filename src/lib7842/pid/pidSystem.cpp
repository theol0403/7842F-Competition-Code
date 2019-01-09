#include "pidSystem.hpp"

namespace lib7842
{

  PID::PID(double Kp, double Kd)
  : m_timer(),
  m_Kp(Kp),
  m_Kd(Kd)
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

    return finalPower;
  }


  double PID::calculate(double wantedRPM, double currentRPM)
  {
    return calculateErr(wantedRPM - currentRPM);
  }


  double PID::getError()
  {
    return m_Error;
  }


}
