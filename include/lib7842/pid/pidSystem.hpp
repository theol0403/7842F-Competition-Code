#pragma once
#include "main.h"
#include "lib7842/other/timingSystem.hpp"
#include "lib7842/other/miscUtils.hpp"


namespace lib7842
{
  class PID
  {
  private:

    double m_Kp = 0;
    double m_Kd = 0;

    okapi::EmaFilter m_dEma;

    double m_Error = 0;
    double m_lastError = 0;
    double m_lastTime = 0;
    double m_derivative = 0;

    Timer m_timer;

    SettledUtil m_settledUtil;
    bool m_isSettled = false;


  public:

    PID(double, double, double, double, double, QTime);

    double calculateErr(double);
    double calculate(double, double);

    double getError();

    void reset();

    bool isSettled();

  };
}
