#include "rpmSystem.hpp"


namespace lib7842
{


  rpmMeasure::rpmMeasure(int topEncoderPort, bool reverseEncoder, double ticksPerRev, double flywheelEncoderRatio)
  : m_timer(), m_encoder(topEncoderPort, topEncoderPort+1, reverseEncoder),
  m_ticksPerRev(ticksPerRev), m_flywheelEncoderRatio(flywheelEncoderRatio), m_minRefresh(20)
  {
    m_lastTime = m_timer.elapsed();
    m_lastEncoder = m_encoder.get_value();
  }


  int rpmMeasure::calculate()
  {
    //Calculate the amount of ms since the last time this function was run
    m_deltaTime = m_timer.elapsed() - m_lastTime;
    //If it was ran very recently just return the last RPM
    if(m_deltaTime < m_minRefresh) {return m_RPM;}

    //Calculate amount of ticks since the function was last run
    m_deltaEncoder = m_encoder.get_value() - m_lastEncoder;

    if(m_deltaEncoder == 0.0) //If sensor was 0, return 0
    {
      m_RPM = 0;
    }
    else
    {
      //Calculate the RPM based off time and tick interval
      m_RPM = (60000 / m_deltaTime) * (m_deltaEncoder/m_ticksPerRev) * m_flywheelEncoderRatio;
    }

    //Timestamp the last time this function was run and the encoder position
    m_lastTime += m_deltaTime;
    m_lastEncoder += m_deltaEncoder;

    //Return the RPM to whatever is calling this function
    return m_RPM;
  }
}
